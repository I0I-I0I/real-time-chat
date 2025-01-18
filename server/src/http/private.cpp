#include <regex>
#include "../logger/logger.h"
#include "./http.h"

HttpPathStruct Http::get_path(std::string path) {
	HttpPathStruct http_path;

	std::regex path_regex(R"((/[A-Za-z0-9.!$%&'*+=^_`{|}~-]*))");
	std::smatch path_matches;

	for (int i = 0;; i++) {
		if (path[0] == '?') break;
		if (std::regex_search(path, path_matches, path_regex)) {
			http_path.path.push_back(path_matches[1].str());
		}
		path = path_matches.suffix().str();
		if (path.empty()) return http_path;
	}

	std::regex params_regex(R"((\w+)=([^&]+))");
	std::smatch params_matches;

	while (true) {
		if (std::regex_search(path, params_matches, params_regex))
			http_path.params[params_matches[1].str()] = params_matches[2].str();
		if (path[0] == '#') break;
		path = params_matches.suffix().str();
		if (path.empty()) break;
	}

	return http_path;
}

std::string Http::to_send(HttpResponseStruct http) {
    std::string response = "";

	response += "HTTP/1.1 " + http.status + " \r\n";

	http.headers["Access-Control-Allow-Origin"] = "*";
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
	logger("Request:", "INFO");
	logger(" \tMethod: " + http.method, "INFO");
    std::string paths = "";
	for (auto& path : http.url.path)
        paths += path;
    logger(" \tPath: " + paths, "INFO");
	for (auto& param : http.url.params)
		logger(" \tParams: " + param.first + " = " + param.second, "INFO");
	logger(" \tVersion: " + http.version, "LOG");
	logger(" \tHeaders:", "LOG");
	for (auto& header : http.headers)
		logger(" \t" + header.first + ": " + header.second, "LOG");
	logger(" \tBody: " + http.body, "LOG");
}

void Http::log(HttpResponseStruct http) {
	logger("Response:", "INFO");
	logger(" \tStatus: " + http.status, "INFO");
	logger(" \tBody: " + http.body, "LOG");
	logger(" \tHeaders:", "LOG");
	for (auto& header : http.headers)
		logger(" \t" + header.first + ": " + header.second, "LOG");
}
