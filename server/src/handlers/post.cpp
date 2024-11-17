#include <string>
#include "../../lib/json.hpp"
#include "../http/http.h"
#include "../db/db.h"
#include "../config.h"
#include "./handlers.h"

using json = nlohmann::json;

std::string HandlerOn::post(const HttpRequestStruct& http) {
	DB db(PATH_TO_DB);

	if (http.url.path[0] != "db")
		return Http::response(400, "Unknown parameter in url");

	if (http.headers.at("Content-Type") != "application/json")
		return Http::response(400, "Unknown Content-Type");

	std::string table = http.url.path[1];
	DBDataListStruct data_list = json::parse(http.body);
	DBResponseStruct response = db.insert_data(table, data_list);

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
