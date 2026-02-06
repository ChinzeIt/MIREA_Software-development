#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <chrono>
#include <cstring>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include <fstream>

using namespace std::chrono;
using clock_type = std::chrono::steady_clock;

static std::string hex(const std::vector<unsigned char>& b) {
    std::ostringstream oss;
    for (unsigned char c : b) oss << std::hex << std::setw(2) << std::setfill('0') << (int)c;
    return oss.str();
}

static std::string rand_hex(int bytes = 16) {
    std::vector<unsigned char> buf(bytes);
    if (!RAND_bytes(buf.data(), bytes)) {
        for (int i = 0; i < bytes; ++i) buf[i] = (unsigned char)(std::rand() & 0xFF);
    }
    return hex(buf);
}

static std::string url_decode(const std::string& s) {
    std::string out;
    char ch;
    int i, ii;
    for (i=0; i<s.length(); i++) {
        if (s[i] == '%') {
            sscanf(s.substr(i+1,2).c_str(), "%x", &ii);
            ch = static_cast<char>(ii);
            out += ch;
            i += 2;
        } else if (s[i] == '+') {
            out += ' ';
        } else {
            out += s[i];
        }
    }
    return out;
}

static std::map<std::string,std::string> parse_query(const std::string& q) {
    std::map<std::string,std::string> m;
    size_t pos = 0;
    while (pos < q.size()) {
        size_t amp = q.find('&', pos);
        if (amp == std::string::npos) amp = q.size();
        size_t eq = q.find('=', pos);
        if (eq != std::string::npos && eq < amp) {
            std::string k = q.substr(pos, eq-pos);
            std::string v = q.substr(eq+1, amp-eq-1);
            m[url_decode(k)] = url_decode(v);
        }
        pos = amp + 1;
    }
    return m;
}

static bool pbkdf2_hash(const std::string& password, const std::vector<unsigned char>& salt,
                        int iterations, std::vector<unsigned char>& out) {
    const EVP_MD* md = EVP_sha256();
    int dklen = 32;
    out.resize(dklen);
    return PKCS5_PBKDF2_HMAC(password.c_str(), password.length(),
                             salt.data(), salt.size(),
                             iterations, md, dklen, out.data()) == 1;
}

struct User {
    std::string username;
    std::vector<unsigned char> salt;
    std::vector<unsigned char> hash;
    int iterations;
    int failed_count = 0;
    time_point<clock_type> locked_until = time_point<clock_type>::min();
    int backoff_level = 0;
};

std::mutex users_mtx;
std::map<std::string, User> users;

static void create_test_user() {
    auto trim = [](std::string s) {
        size_t a = s.find_first_not_of(" \t\r\n");
        if (a == std::string::npos) return std::string();
        size_t b = s.find_last_not_of(" \t\r\n");
        return s.substr(a, b-a+1);
    };

    std::vector<std::pair<std::string,std::string>> creds;

    {
        std::ifstream credf("logi.txt");
        if (credf) {
            std::string line;
            while (std::getline(credf, line)) {
                line = trim(line);
                if (line.empty()) continue;
                size_t col = line.find(':');
                if (col == std::string::npos) continue;
                std::string user = trim(line.substr(0, col));
                std::string pass = trim(line.substr(col+1));
                if (!user.empty() && !pass.empty()) creds.emplace_back(user, pass);
            }
        }
    }

    int created = 0;
    {
        std::lock_guard<std::mutex> lk(users_mtx);
        for (auto &p : creds) {
            const std::string &username = p.first;
            const std::string &password = p.second;

            if (username.empty() || password.empty()) continue;
            if (users.find(username) != users.end()) {
                continue;
            }

            User u;
            u.username = username;
            u.iterations = 100000;
            u.salt.resize(16);
            if (!RAND_bytes(u.salt.data(), (int)u.salt.size())) {
                for (size_t i = 0; i < u.salt.size(); ++i) u.salt[i] = static_cast<unsigned char>(std::rand() & 0xFF);
            }

            if (!pbkdf2_hash(password, u.salt, u.iterations, u.hash)) {
                continue;
            }

            users[u.username] = std::move(u);
            ++created;
        }
    }
}

struct Session {
    std::string sid;
    std::string user_token;
    time_point<clock_type> expires;
};

std::mutex sessions_mtx;
std::map<std::string, Session> sessions;

static std::string make_session(int lifetime_seconds = 300) {
    Session s;
    s.sid = rand_hex(16);
    s.user_token = rand_hex(12);
    s.expires = clock_type::now() + seconds(lifetime_seconds);

    std::lock_guard<std::mutex> lk(sessions_mtx);
    sessions[s.sid] = s;
    return s.sid;
}

static std::string get_token_for_sid(const std::string& sid) {
    std::lock_guard<std::mutex> lk(sessions_mtx);
    auto it = sessions.find(sid);
    if (it == sessions.end()) return "";
    if (it->second.expires < clock_type::now()) {
        sessions.erase(it);
        return "";
    }
    return it->second.user_token;
}

struct IPState {
    std::vector<time_point<clock_type>> hits;
};

std::mutex ips_mtx;
std::map<std::string, IPState> ip_states;

static bool ip_allow(const std::string& ip, int N = 10, int window_seconds = 60) {
    std::lock_guard<std::mutex> lk(ips_mtx);
    auto& s = ip_states[ip];

    auto now = clock_type::now();
    while (!s.hits.empty() &&
           duration_cast<seconds>(now - s.hits.front()).count() > window_seconds) {
        s.hits.erase(s.hits.begin());
    }
    if ((int)s.hits.size() >= N) return false;

    s.hits.push_back(now);
    return true;
}

static std::string http_response(
        const std::string& body,
        const std::string& status="200 OK",
        const std::vector<std::pair<std::string,std::string>>& headers = {}
) {
    std::ostringstream oss;
    oss << "HTTP/1.1 " << status << "\r\n";
    oss << "Content-Type: text/html; charset=utf-8\r\n";
    oss << "Content-Length: " << body.size() << "\r\n";
    for (auto &h : headers) {
        oss << h.first << ": " << h.second << "\r\n";
    }
    oss << "\r\n" << body;
    return oss.str();
}

static std::string make_login_form(const std::string& sid, const std::string& token) {
    std::ostringstream o;
    o << "<html><body>\n";
    o << "<h2>Brute demo (safe)</h2>\n";
    o << "<p>Session SID: " << sid << "</p>\n";
    o << "<form method=\"get\" action=\"/vulnerabilities/brute/\">\n";
    o << "Username: <input name=\"username\"><br>\n";
    o << "Password: <input name=\"password\" type=\"password\"><br>\n";
    o << "<input type=\"hidden\" name=\"user_token\" value=\"" << token << "\">\n";
    o << "<input type=\"submit\" name=\"Login\" value=\"Login\">\n";
    o << "</form>\n";
    o << "</body></html>";
    return o.str();
}

static std::string header_get_value(const std::string& headers, const std::string& name) {
    std::istringstream iss(headers);
    std::string line;
    std::string lname = name;
    for (char& c : lname) c = std::tolower(c);

    while (std::getline(iss, line)) {
        if (line.size() == 0) continue;

        auto pos = line.find(':');
        if (pos == std::string::npos) continue;

        std::string hn = line.substr(0, pos);
        std::string hv = line.substr(pos + 1);

        std::string key;
        for (char ch : hn) key += std::tolower(ch);

        if (key == lname) {
            size_t start = hv.find_first_not_of(" \t");
            size_t end = hv.find_last_not_of("\r\n ");
            if (start == std::string::npos) return "";
            return hv.substr(start, end - start + 1);
        }
    }
    return "";
}

static std::string cookie_get_sid(const std::string& cookie_header) {
    size_t pos = 0;
    while (pos < cookie_header.size()) {
        size_t sem = cookie_header.find(';', pos);
        if (sem == std::string::npos) sem = cookie_header.size();

        size_t eq = cookie_header.find('=', pos);
        if (eq != std::string::npos && eq < sem) {
            std::string k = cookie_header.substr(pos, eq-pos);
            std::string v = cookie_header.substr(eq+1, sem-eq-1);

            auto trim = [](std::string s){
                size_t a = s.find_first_not_of(" \t");
                if (a == std::string::npos) return std::string();
                size_t b = s.find_last_not_of(" \t");
                return s.substr(a, b-a+1);
            };

            k = trim(k);
            v = trim(v);

            if (k == "SID") return v;
        }

        pos = sem + 1;
    }
    return "";
}

static std::string handle_brute_get(
        const std::string& query,
        const std::string& cookie_header,
        const std::string& client_ip
) {
    if (query.empty()) {
        std::string sid = make_session();
        std::string token = get_token_for_sid(sid);

        auto body = make_login_form(sid, token);

        return http_response(
            body, "200 OK",
            {
                {"Set-Cookie", "SID=" + sid + "; Path=/; HttpOnly; SameSite=Lax"}
            }
        );
    }

    if (!ip_allow(client_ip, 10, 60)) {
        return http_response(
            "<html><body><h3>Too many requests</h3></body></html>",
            "429 Too Many Requests"
        );
    }

    auto params = parse_query(query);

    if (!params.count("Login"))
        return http_response("<html><body>Bad request</body></html>","400 Bad Request");

    std::string sid = cookie_get_sid(cookie_header);
    if (sid.empty())
        return http_response("<html><body>No SID cookie. Reload form.</body></html>", "400 Bad Request");

    std::string expected_token = get_token_for_sid(sid);
    std::string provided_token = params.count("user_token") ? params["user_token"] : "";

    if (expected_token.empty() || provided_token != expected_token)
        return http_response("<html><body>Invalid token</body></html>", "403 Forbidden");

    std::string username = params["username"];
    std::string password = params["password"];

    if (username.empty())
        return http_response("<html><body>Username required</body></html>", "400 Bad Request");

    bool ok = false;

    {
        std::lock_guard<std::mutex> lk(users_mtx);

        auto it = users.find(username);

        if (it != users.end()) {
            User& u = it->second;

            if (u.locked_until > clock_type::now()) {
                int rem = duration_cast<seconds>(u.locked_until - clock_type::now()).count();
                return http_response(
                    "<html><body>Account locked. Retry in " + std::to_string(rem) + " sec.</body></html>",
                    "403 Forbidden"
                );
            }

            std::vector<unsigned char> attempt;
            pbkdf2_hash(password, u.salt, u.iterations, attempt);

            if (attempt == u.hash) {
                ok = true;
                u.failed_count = 0;
                u.backoff_level = 0;
                u.locked_until = time_point<clock_type>::min();
            } else {
                u.failed_count++;
                if (u.failed_count >= 3) {
                    u.backoff_level++;
                    int lock_seconds = 60 * (1 << std::min(u.backoff_level-1, 5));
                    u.locked_until = clock_type::now() + seconds(lock_seconds);
                }
            }
        } else {
            std::vector<unsigned char> fake_salt(16);
            RAND_bytes(fake_salt.data(), 16);
            std::vector<unsigned char> fake_hash;
            pbkdf2_hash(password, fake_salt, 100000, fake_hash);
        }
    }

    if (ok) {
        return http_response(
            "<html><body><h3>Login successful</h3></body></html>",
            "200 OK",
            {
                {"Set-Cookie","AUTH=1; Path=/; HttpOnly; Max-Age=3600; SameSite=Lax"}
            }
        );
    }

    return http_response("<html><body>Invalid credentials</body></html>");
}

static std::string read_all(int fd) {
    char buf[4096];
    ssize_t n = recv(fd, buf, sizeof(buf)-1, 0);
    if (n <= 0) return "";
    return std::string(buf, n);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <port>\n";
        return 1;
    }

    int port = std::stoi(argv[1]);

    RAND_poll();
    create_test_user();

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("socket"); return 1; }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return 1;
    }

    if (listen(server_fd, 50) < 0) {
        perror("listen");
        return 1;
    }

    std::cout << "Auth server listening on port " << port
              << " (test user: admin / password)" << std::endl;

    while (true) {
        struct sockaddr_in client_addr{};
        socklen_t len = sizeof(client_addr);

        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &len);
        if (client_fd < 0) continue;

        char ipbuf[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, ipbuf, sizeof(ipbuf));
        std::string client_ip(ipbuf);

        std::thread([client_fd, client_ip](){
            std::string req = read_all(client_fd);
            if (req.empty()) {
                close(client_fd);
                return;
            }

            std::istringstream iss(req);
            std::string request_line;
            std::getline(iss, request_line);

            if (!request_line.empty() && request_line.back() == '\r')
                request_line.pop_back();

            std::string headers;
            std::string line;
            while (std::getline(iss, line)) {
                if (line == "\r" || line == "") break;
                headers += line + "\n";
            }

            std::istringstream rl(request_line);
            std::string method, fullpath, httpver;
            rl >> method >> fullpath >> httpver;

            std::string path = fullpath;
            std::string query;

            size_t pos = fullpath.find('?');
            if (pos != std::string::npos) {
                path = fullpath.substr(0, pos);
                query = fullpath.substr(pos+1);
            }

            std::string cookie_header = header_get_value(headers, "Cookie");
            std::string resp;

            if (method == "GET" && path == "/vulnerabilities/brute/") {
                resp = handle_brute_get(query, cookie_header, client_ip);
            } else {
                resp = http_response("<html><body>Not Found</body></html>", "404 Not Found");
            }

            send(client_fd, resp.c_str(), resp.size(), 0);
            close(client_fd);

        }).detach();
    }
}

// g++ auth.cpp -o auth -lssl -lcrypto -pthread
// ./auth 8080