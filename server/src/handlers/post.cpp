#include <string>
#include "../../lib/json.hpp"
#include "../http/http.h"
#include "../db/db.h"
#include "../config.h"
#include "./handlers.h"
#include "utils/utils.h"

using json = nlohmann::json;

std::string HandlerOn::post(const HttpRequestStruct& http) {
	if (http.url.path.at(0) != "/api")
		return Http::response(400, "You missed '/api'");

    if (http.url.path.size() < 3)
		return Http::response(400, "You missed table name or something");

	if (http.headers.at("content-type") != "application/json")
		return Http::response(400, "Unknown Content-Type, you can only pass 'application/json'");

	DB db(DB_PATH);

	std::string table = http.url.path[2].substr(1);
	DBDataListStruct data = json::parse(http.body);

	DBResponseStruct response;
	if (http.url.params.find("type") != http.url.params.end()
			&& http.url.params.at("type") == "check")
		response = db.check_data(table, data[0]["login"], data[0]["password"]);
	else
		response = db.insert_data(table, data);


	return Http::response(
		response.status,
		get_resp_body(response),
		{
			{ "Content-Type", "application/json" },
			{ "Connection", "close" }
		}
	);
}
