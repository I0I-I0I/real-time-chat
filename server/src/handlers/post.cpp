#include <any>
#include <string>
#include "../../lib/json.hpp"
#include "../http/http.h"
#include "../db/db.h"
#include "../config.h"
#include "./handlers.h"

using json = nlohmann::json;

std::string HandlerOn::post(const std::any& data) {
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

	json data_list = json::parse(http.body);
	DBResponseStruct response = db.insert_data(
		http.path.type,
		data_list
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
