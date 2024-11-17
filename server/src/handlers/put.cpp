#include <string>
#include "../http/http.h"
#include "../db/db.h"
#include "../../lib/json.hpp"
#include "../config.h"
#include "./handlers.h"

using json = nlohmann::json;

std::string HandlerOn::put(const HttpRequestStruct& http) {
	DB db(PATH_TO_DB);

	if (http.url.params.find("id") == http.url.params.end())
		return Http::response(400, "Missing 'id'");
	std::string id = http.url.params.at("id");

	if (http.url.path[0] != "db")
		return Http::response(400, "Unknown parameter in url");
	std::string table = http.url.path[1];

	if (http.headers.at("Content-Type") != "application/json")
		return Http::response(400, "Unknown Content-Type");

	DBDataListStruct recv_body = json::parse(http.body);
	DBResponseStruct response = db.update_data(table, id, recv_body[0]);

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
