#include <iostream>
#include <string>
#include <regex>
#include "../logger/logger.h"
#include "./http.h"

std::string Http::to_send(HttpResponseStruct http) {
	std::string response = "";

	response += "HTTP/1.1 " + http.status + " \r\n";

	http.headers["Access-Control-Allow-Origin"] = "*";
	http.headers["Access-Control-Allow-Methods"] = "GET, POST, OPTIONS";
	for (const auto& header : http.headers)
		response += header.first + ": " + header.second + "\r\n";
	response += "\r\n";
	response += http.body;

	return response;
}

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
			http.path = get_path(matches[2].str());
            http.version = matches[3].str();
        }

    HttpHeadersStruct headers;
    while (std::getline(stream, line) && line != "\r")
        if (std::regex_search(line, matches, header_pattern))
            if (matches.size() == 3) {
                std::string key = matches[1].str();
                std::string value = matches[2].str();
                headers[key] = value;
            }
	http.headers = headers;

    std::string body;
    while (std::getline(stream, line))
        body += line + "\r\n";
	http.body = body;

	std::cout << "[HTTP] Method: " + http.method << std::endl;
	std::cout << "[HTTP] Path: " + http.path.path << std::endl;
	std::cout << "[HTTP] Type: " + http.path.type << std::endl;
	std::cout << "[HTTP] Version: " + http.version << std::endl;
	std::cout << "[HTTP] Headers:" << std::endl;
	for (auto& header : http.headers)
		std::cout << "[HTTP] \t" + header.first + ": " + header.second << std::endl;
	std::cout << "[HTTP] Body: " + http.body << std::endl;

	return http;
}
