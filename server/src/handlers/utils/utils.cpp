#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "../../config.h"
#include "../../http/http.h"
#include "../../db/db.h"
#include "./utils.h"

bool includes(const std::vector<std::string>& arr, const std::string& str) {
	for (int i = 0; i < arr.size(); i++)
		if (arr.at(i) == str)
			return true;
	return false;
}

std::map<std::string, std::string> get_headers_of_extantion(const std::string& extantion) {
	std::vector<std::string> image_types = {
		"png",
		"jpg",
		"jpeg",
		"gif",
		"svg",
		"webp",
		"ico",
		"avif",
	};
	std::map<std::string, std::string> headers;

	if (extantion == "js"){
		headers["Content-Type"] = "text/javascript";
		headers["X-Content-Type-Options"] = "script";
	} else if (extantion == "css") {
		headers["Content-Type"] = "text/css";
		headers["X-Content-Type-Options"] = "style";
	} else if (includes(image_types, extantion)) {
		headers["Content-Type"] = "image/" + extantion;
	}

	return headers;
}

GetFileStruct get_file(const std::string& path) {
	std::ifstream file(FRONTED_PATH + path);

	if (!file.is_open())
		return {
			"File not found",
			path,
			""
		};

	std::stringstream buffer;
	buffer << file.rdbuf();
	return {
		buffer.str(),
		path,
		path.substr(path.find_last_of(".") + 1)
	};
}

std::string get_resp_for_file(const HttpRequestStruct& http) {
	std::string file_path;
	if (http.url.path.at(0) == "/")
		file_path = "/index.html";
	else
		for (int i = 0; i < http.url.path.size(); i++)
			file_path += http.url.path.at(i);

	GetFileStruct file = get_file(file_path);
	std::map<std::string, std::string> headers = {
		{ "Content-Length", std::to_string(file.body.size()) },
	};

	std::map<std::string, std::string> headers_of_ext = get_headers_of_extantion(file.extantion);
	for (const auto& [key, value] : headers_of_ext)
		headers[key] = value;

	return Http::response(200, file.body, headers);
}

std::string get_resp_body(const DBResponseStruct& response) {
	json body = {
		{ "status", response.body.status },
		{ "data", response.body.data },
		{ "message", response.body.msg },
	};
	return body.dump();
}
