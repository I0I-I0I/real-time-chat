#include <string>
#include "../http/http.h"
#include "../db/db.h"
#include "../../lib/json.hpp"
#include "../config.h"
#include "./handlers.h"

using json = nlohmann::json;

std::string HandlerOn::del(const HttpRequestStruct& http) {
	DB db(DB_PATH);

	if (http.url.params.find("id") == http.url.params.end())
		return Http::response(400, "Missing 'id'");
	std::string id = http.url.params.at("id");

	std::string table = http.url.path[2];
	DBResponseStruct response = db.delete_data(table, id);

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
