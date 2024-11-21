#include <fstream>
#include <iostream>
#include <string>
#include "../http/http.h"
#include "../db/db.h"
#include "../../lib/json.hpp"
#include "../config.h"
#include "./handlers.h"

using json = nlohmann::json;

struct GetFileStruct {
	std::string file;
	std::string path;
	std::string extantion;
};

GetFileStruct get_file(std::string path) {
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

bool includes(std::vector<std::string> arr, std::string str) {
	for (int i = 0; i < arr.size(); i++)
		if (arr.at(i) == str)
			return true;
	return false;
}

std::string HandlerOn::get(const HttpRequestStruct& http) {
	DB db(PATH_TO_DB);

	if (http.url.path.at(0) != "/api") {
		std::string path_file;
		if (http.url.path.at(0) == "/")
			path_file = "/index.html";
		else
			for (int i = 0; i < http.url.path.size(); i++)
				path_file += http.url.path.at(i);

		GetFileStruct file = get_file(path_file);
		std::map<std::string, std::string> headers = {
			{ "Content-Length", std::to_string(file.file.size()) },
		};

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
		std::string content_type = "text/html";

		if (file.extantion == "js"){
			content_type = "text/javascript";
			headers["X-Content-Type-Options"] = "script";
		} else if (file.extantion == "css") {
			content_type = "text/css";
			headers["X-Content-Type-Options"] = "style";
		} else if (includes(image_types, file.extantion)) {
			content_type = "image/" + file.extantion;
		}
		headers["Content-Type"] = content_type;

		return Http::response(200, file.file, headers);
	}

	std::string id = "";
	if (http.url.params.find("id") != http.url.params.end())
		id = http.url.params.at("id");

	if (http.url.path.size() < 3)
		return Http::response(400, "Something missing in URL");

	std::string table = http.url.path[2].substr(1);
	DBResponseStruct response = db.get_data(table, id);

	json body = {
		{ "status", response.body.status },
		{ "data", response.body.data },
		{ "message", response.body.msg },
	};

	return Http::response(response.status, body.dump(), {
		{ "Content-Type", "application/json" },
		{ "Connection", "close" }
	});
}
