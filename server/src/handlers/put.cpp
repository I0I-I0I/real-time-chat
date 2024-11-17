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
	DBDataListStruct recv_body = json::parse(http.body);
	DBResponseStruct response = db.update_data(table, id, recv_body[0]);

	std::string body = json(response.data).dump();

	return Http::response(response.status, body, {
		{ "Content-Type", "application/json" }
	});
}
