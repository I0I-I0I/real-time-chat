#include <string>
#include "../http/http.h"
#include "../db/db.h"
#include "../../lib/json.hpp"
#include "../config.h"
#include "./handlers.h"

using json = nlohmann::json;

std::string HandlerOn::get(const HttpRequestStruct& http) {
	DB db(PATH_TO_DB);

	std::string id = "";
	if (http.path.params.find("id") != http.path.params.end())
		id = http.path.params.at("id");

	std::string table = http.path.type;
	DBResponseStruct response = db.get_data(table, id);

	std::string body = json(response.data).dump();
	StatusStruct status = response.status;

	return Http::response(status.code, status.msg, body, {
		{ "Content-Type", "application/json" }
	});
}
