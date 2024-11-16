#include <string>
#include "../../lib/json.hpp"
#include "../http/http.h"
#include "../db/db.h"
#include "../config.h"
#include "./handlers.h"

using json = nlohmann::json;

std::string HandlerOn::post(const HttpRequestStruct& http) {
	DB db(PATH_TO_DB);

	std::string table = http.path.type;
	DBDataListStruct data_list = json::parse(http.body);
	DBResponseStruct response = db.insert_data(table, data_list);

	std::string body = json(response.data).dump();

	return Http::response(response.status, body, {
		{ "Content-Type", "application/json" }
	});
}
