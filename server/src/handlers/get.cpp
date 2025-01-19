#include <string>
#include "../http/http.h"
#include "../db/db.h"
#include "../../lib/json.hpp"
#include "../config.h"
#include "./utils/utils.h"
#include "./handlers.h"

using json = nlohmann::json;

HttpResponseStruct HandlerOn::get(const HttpRequestStruct& http) {
    HttpHeadersStruct headers = {
        { "content-type", "application/json" },
        { "connection", "close" }
    };

	if (http.url.path.at(0) != "/api") {
        if ((http.headers.find("connection") != http.headers.end())
                && (http.headers.at("connection") == "keep-alive")) {
            headers["connection"] = "keep-alive";
            headers["keep-alive"] = "timeout=15, max=100";
        } else {
            headers["connection"] = "close";
        }
		return get_resp_for_file(http, headers);
    }

	if (http.url.path.size() < 3)
		return Http::response(400, "Something missing in URL", headers);

	DB db(DB_PATH);

    DBResponseStruct response;
    std::string table = http.url.path[2].substr(1);
    if (http.url.params.find("login") != http.url.params.end()) {
        response = db.get_data_by("login", table, http.url.params.at("login"), "id, login, username, created_at");
    } else if (http.url.params.find("id") != http.url.params.end()) {
        response = db.get_data_by("id", table, http.url.params.at("id"));
    } else {
        response = db.get_data(table);
    }

	return Http::response(response.status, get_resp_body(response), headers);
}
