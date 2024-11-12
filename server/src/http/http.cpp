#include <string>
#include <regex>
#include "../socket/logger/logger.h"
#include "./http.h"

std::string Http::create(HttpResponseStruct http) {
	std::string response = "";

	response += "HTTP/1.1 " + http.status + " \r\n";

	http.headers["Access-Control-Allow-Origin"] = "*";
	http.headers["Access-Control-Allow-Methods"] = "GET, POST, OPTIONS";
	http.headers["Access-Control-Allow-Headers"] = "Content-Type, Authorization";
	for (const auto& header : http.headers)
		response += header.first + ": " + header.second + "\r\n";
	response += "\r\n";
	response += http.body;

	return response;
}

HttpRequestStruct Http::parce(std::string request) {
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
            http.path = matches[2].str();
            http.version = matches[3].str();
        }

    std::map<std::string, std::string> headers;
    while (std::getline(stream, line) && !line.empty())
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

	log(http);

	return http;
}

void Http::log(HttpRequestStruct http) {
	logger("Method: " + http.method, "HTTP");
	logger("Path: " + http.path, "HTTP");
	logger("Version: " + http.version, "HTTP");
	logger("Headers: ", "HTTP");
	for (auto& header : http.headers)
		logger("\t" + header.first + ": " + header.second, "HTTP");
	logger("Body: " + http.body, "HTTP");
}
