#include <any>
#include <iostream>
#include <string>
#include <regex>
#include "./http.h"

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

	Http::log(http);

	return http;
}

std::string Http::response(int code, std::string status, std::string body, HttpHeadersStruct headers) {
	HttpResponseStruct http;
	http.status = std::to_string(code) + " " + status;
	http.headers = headers;
	http.body = body;

	return Http::to_send(http);
}

TestOnHttpStruct Http::test_on_http(const std::any& data) {
	if (data.type() != typeid(HttpRequestStruct))
		return {
			.is_error = true,
			.response = Http::response(
			400,
			"Bad request",
			"Something strange",
			{})
		};

	return {
		false, std::any_cast<HttpRequestStruct>(data)
	};
}
