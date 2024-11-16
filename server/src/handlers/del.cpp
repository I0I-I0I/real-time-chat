#include <string>
#include "../http/http.h"
#include "../db/db.h"
#include "../../lib/json.hpp"
#include "../config.h"
#include "./handlers.h"

using json = nlohmann::json;

std::string HandlerOn::del(const HttpRequestStruct& http) {
	DB db(PATH_TO_DB);

	if (http.path.params.find("id") == http.path.params.end())
		return Http::response(400, "Missing 'id'");

	std::string id = http.path.params.at("id");
	std::string table = http.path.type;
	DBResponseStruct response = db.delete_data(table, id);

	std::string body = json(response.data).dump();

	return Http::response(response.status, body, {
		{ "Content-Type", "application/json" }
	});
}
