#include <iostream>
#include <curl/curl.h>
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>

using namespace std;
using json = nlohmann::json;

// Callback function to handle the response data
size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <DATA_TO_POST>" << endl;
        return 1;
    }
    // Read URL from /etc/logsvc (first non-empty line)
    const char* cfg_path = "/etc/logsvc";
    ifstream cfg(cfg_path);
    if (!cfg) {
        cerr << "Failed to open " << cfg_path << " to read target URL" << endl;
        return 1;
    }

    string url;
    string line;
    while (std::getline(cfg, line)) {
        // trim whitespace
        line.erase(line.begin(), find_if(line.begin(), line.end(), [](unsigned char ch) { return !std::isspace(ch); }));
        line.erase(find_if(line.rbegin(), line.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), line.end());
        if (!line.empty()) {
            url = line;
            break;
        }
    }

    if (url.empty()) {
        cerr << "No URL found in " << cfg_path << endl;
        return 1;
    }

    // Build JSON payload with current timestamp and provided data
    string message = argv[1];
    // Get current time in UTC ISO-8601 (YYYY-MM-DDTHH:MM:SSZ)
    auto now = std::chrono::system_clock::now();
    time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm{};
    gmtime_r(&t, &tm);
    std::ostringstream tss;
    tss << std::put_time(&tm, "%Y-%m-%dT%H:%M:%SZ");
    string iso_ts = tss.str();

    json payload;
    payload["timestamp"] = iso_ts;
    payload["message"] = message;
    string postData = payload.dump();
    string response;

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL* curl = curl_easy_init();

    if (!curl) {
        cerr << "Failed to initialize curl" << endl;
        curl_global_cleanup();
        return 1;
    }

    struct curl_slist* headers = nullptr;
    // We post JSON with timestamp
    headers = curl_slist_append(headers, "Content-Type: application/json");

    // Set the URL and POST data
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)postData.size());

    // Set headers
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // Capture response
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    // Follow redirects
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    // Perform the request
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 1;
    }

    // Attempt to parse the response as JSON
    try {
        json j = json::parse(response);
        cout << "Parsed JSON response:\n" << j.dump(4) << endl;
    } catch (const json::parse_error& e) {
        // Not JSON — print raw response
        cout << "Response from " << url << " (raw):\n" << endl;
        cout << response << endl;
    }

    // Cleanup
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return 0;
}
