#include <string>
#include "../http/http.h"
#include "../db/db.h"
#include "../../lib/json.hpp"
#include "../config.h"
#include "./utils/utils.h"
#include "./handlers.h"

using json = nlohmann::json;

std::string HandlerOn::get(const HttpRequestStruct& http) {
	if (http.url.path.at(0) != "/api")
		return get_resp_for_file(http);

	if (http.url.path.size() < 3)
		return Http::response(400, "Something missing in URL");

	DB db(DB_PATH);

	std::string id = "";
	if (http.url.params.find("id") != http.url.params.end())
		id = http.url.params.at("id");

	std::string table = http.url.path[2].substr(1);
	DBResponseStruct response = db.get_data(table, id);

	return Http::response(
		response.status,
		get_resp_body(response),
		{
			{ "Content-Type", "application/json" },
			{ "Connection", "close" }
		}
	);
}
