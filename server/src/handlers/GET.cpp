#include <any>
#include <string>
#include "../http/http.h"
#include "../db/db.h"
#include "../../lib/json.hpp"
#include "../config.h"
#include "./handlers.h"

using json = nlohmann::json;

std::string HandlerOn::get(const std::any& data) {
	if (data.type() != typeid(HttpRequestStruct)) return Http::to_send({
		.status = "400 Bad Request",
		.headers = {
			{ "Content-Type", "text/plain" },
			{ "Connection", "close" }
		},
		.body = "Something strange",
	});

	HttpRequestStruct http = std::any_cast<HttpRequestStruct>(data);
	DB db(PATH_TO_DB);

	DBResponseStruct response = db.get_data(
		http.path.type,
		http.path.params["id"] != "" ? http.path.params["id"] : ""
	);

	std::string body = json(response.data).dump();
	std::string status = response.status;

	return Http::to_send({
		.status = status,
		.headers = {
			{ "Content-Type", "application/json" },
		},
		.body = body,
	});
}
