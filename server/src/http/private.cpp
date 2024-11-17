#include <regex>
#include "../logger/logger.h"
#include "./http.h"

HttpPathStruct Http::get_path(std::string path) {
	HttpPathStruct http_path;

	std::regex path_regex(R"(/(\w+))");
	std::smatch path_matches;

	for (int i = 0;; i++) {
		if (std::regex_search(path, path_matches, path_regex))
			http_path.path.push_back(path_matches[1].str());
		if (path[0] == '?') break;
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
	logger("Request:", "HTTP");
	logger("Method: " + http.method, "\t");
	for (auto& path : http.url.path)
		logger("Path: " + path, "\t");
	for (auto& param : http.url.params)
		logger("Path: " + param.first + ": " + param.second, "\t");
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
