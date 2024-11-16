#include <string>
#include "../http/http.h"
#include "../db/db.h"
#include "../../lib/json.hpp"
#include "../config.h"
#include "./handlers.h"

using json = nlohmann::json;

std::string HandlerOn::put(const HttpRequestStruct& http) {
	DB db(PATH_TO_DB);

	std::string id = http.path.params.at("id");

	std::string table = http.path.type;
	DBDataListStruct recv_body = json::parse(http.body);
	DBResponseStruct response = db.update_data(table, id, recv_body[0]);

	std::string body = json(response.data).dump();
	StatusStruct status = response.status;

	return Http::response(status.code, status.msg, body, {
		{ "Content-Type", "application/json" }
	});
}
