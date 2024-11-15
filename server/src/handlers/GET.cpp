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
		.body = "",
	});

	HttpRequestStruct http = std::any_cast<HttpRequestStruct>(data);

	DB db(PATH_TO_DB);
	json response_body;
	std::string result;
	if (http.path.params["id"] != "") {
		response_body = db.get_data_by_id(http.path.type, http.path.params["id"]);
		result = "[" + response_body.dump() + "]";
	} else {
		response_body = db.get_data(http.path.type);
		result = response_body.dump();
	}

	return Http::to_send({
		.status = "200 OK",
		.headers = {
			{ "Content-Type", "application/json" },
			{ "Connection", "close" }
		},
		.body = result,
	});
}
