#include <regex>
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
	http.headers["Access-Control-Allow-Methods"] = "GET, POST, OPTIONS";
	for (const auto& header : http.headers)
		response += header.first + ": " + header.second + "\r\n";
	response += "\r\n";
	response += http.body;

	return response;
}

