#include <string>
#include <regex>
#include "../config.h"
#include "../../lib/json.hpp"
#include "./http.h"

using json = nlohmann::json;

HttpRequestStruct Http::parce(const std::string& request) {
	HttpRequestStruct http;

	std::regex request_line_pattern(R"((\w+) ([^ ]+) HTTP/(\d.\d))");
    std::regex header_pattern(R"(([^:]+): (.+))");
	std::smatch matches;

    std::istringstream stream(request);
    std::string line;

    std::getline(stream, line);
    if (std::regex_search(line, matches, request_line_pattern))
        if (matches.size() == 4) {
            http.method = matches[1].str();
			http.url = get_path(matches[2].str());
            http.version = matches[3].str();
        }

    HttpHeadersStruct headers;
    while (std::getline(stream, line) && line != "\r") {
        if (std::regex_search(line, matches, header_pattern)) {
            if (matches.size() == 3) {
                std::string key = matches[1].str();
                std::string value = matches[2].str();
                std::transform(key.begin(), key.end(), key.begin(),
                               [](unsigned char c){ return std::tolower(c); });
                std::transform(value.begin(), value.end(), value.begin(),
                               [](unsigned char c){ return std::tolower(c); });
                headers[key] = value;
            }
        }
    }
    http.headers = headers;

    std::string body;
    while (std::getline(stream, line)) {
        body += line + "\r\n";
    }
    http.body = body;

	Http::log(http);

	return http;
}

std::string Http::response(const int& code, const std::string& body, HttpHeadersStruct headers) {
	HttpResponseStruct http;
	http.status = std::to_string(code) + " " + Http::get_status(code);
	if (headers.find("content-type") == headers.end())
		headers["content-type"] = "plain/text";
	http.headers = headers;
	http.body = body;

	return Http::to_send(http);
}
