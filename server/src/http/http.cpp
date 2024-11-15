#include <string>
#include <regex>
#include "../socket/logger/logger.h"
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

HttpPathStruct Http::get_path(std::string path) {
	HttpPathStruct http_path;

	std::regex path_regex(R"(^/(\w+)/(\w+)(?:\?(.*))?$)");
	std::smatch matches;

	if (std::regex_search(path, matches, path_regex)) {
		http_path.path = matches[1].str();
		http_path.type = matches[2].str();

		std::string params_string = matches[3].str();
        std::regex param_regex(R"((\w+)=([^;]+))");
		std::sregex_iterator it(params_string.begin(), params_string.end(), param_regex), end;

		for (; it != end; ++it) {
			std::smatch param_match = *it;
			http_path.params[param_match[1].str()] = param_match[2].str();
		}
	}

	return http_path;
}

HttpRequestStruct Http::parce(std::string request) {
	HttpRequestStruct http;

	std::regex request_line_pattern(R"((\w+) ([^ ]+) HTTP/(\d.\d))");
    std::regex header_pattern(R"(([^:]+): (.+))");
	std::smatch matches;

    std::istringstream stream(request);
    std::string line;

    std::getline(stream, line);
	std::string path;
    if (std::regex_search(line, matches, request_line_pattern))
        if (matches.size() == 4) {
            http.method = matches[1].str();
			http.path = get_path(matches[2].str());
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
	logger("Path: " + http.path.path + " " + http.path.type, "HTTP");
	logger("Version: " + http.version, "HTTP");
	logger("Headers: ", "HTTP");
	for (auto& header : http.headers)
		logger("\t" + header.first + ": " + header.second, "HTTP");
	logger("Body: " + http.body, "HTTP");
}
