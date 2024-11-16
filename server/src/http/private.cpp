#include <regex>
#include "../logger/logger.h"
#include "./http.h"

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

std::string Http::to_send(HttpResponseStruct http) {
	std::string response = "";

	response += "HTTP/1.1 " + http.status + " \r\n";

	http.headers["Access-Control-Allow-Origin"] = "*";
	http.headers["Access-Control-Allow-Methods"] = "GET, POST, PUT, DELETE, PATCH, OPTIONS";
	http.headers["Access-Control-Allow-Headers"] = "Content-Type, Authorization";
	for (const auto& header : http.headers)
		response += header.first + ": " + header.second + "\r\n";
	response += "\r\n";
	response += http.body;

	Http::log(http);

	return response;
}

std::string Http::get_status(int code) {
	std::map<int, std::string> status_codes = {
		{ 200, "OK" },
		{ 400, "Bad request" },
		{ 404, "Not found" },
		{ 405, "Method not allowed" },
		{ 500, "Internal server error" }
	};

	if (status_codes.find(code) != status_codes.end())
		return status_codes.at(code);
	return "Unknown";
}

void Http::log(HttpRequestStruct http) {
	logger("Request:", "HTTP");
	logger("Method: " + http.method, "\t");
	logger("Path: " + http.path.path, "\t");
	for (auto& param : http.path.params)
		logger("Path: " + param.first + ": " + param.second, "\t");
	logger("Type: " + http.path.type, "\t");
	logger("Version: " + http.version, "\t");
	logger("Headers:", "\t");
	for (auto& header : http.headers)
		logger("\t" + header.first + ": " + header.second, "\t");
	logger("Body: " + http.body, "\t");
}

void Http::log(HttpResponseStruct http) {
	logger("Response:", "HTTP");
	logger("Status: " + http.status, "\t");
	logger("Body: " + http.body, "\t");
	logger("Headers:", "\t");
	for (auto& header : http.headers)
		logger("\t" + header.first + ": " + header.second, "\t");
}
