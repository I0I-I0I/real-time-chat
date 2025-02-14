#include <functional>
#include <map>
#include <string>
#include "../../../lib/json.hpp"
#include "../../http/http.h"
#include "../../db/db.h"
#include "../../config.h"
#include "../utils/utils.h"
#include "../handlers.h"
#include "./get.h"

using json = nlohmann::json;

std::map<std::string, OnUrlFunc> paths = {
    { "/users", on_users }
};

HttpResponseStruct HandlerOn::get(const HttpRequestStruct& http) {
    HttpHeadersStruct headers = {
        { "connection", "close" }
    };

	if (http.url.path.at(0) != "/api") return on_file(http, headers);
	if (http.url.path.size() < 3) return Http::response(400, "Something missing in URL");

    headers["content-type"] = "application/json";

	DB db(DB_PATH);

    DBResponseStruct response;
    std::string path = http.url.path[2];
    if (paths.find(path) != paths.end()) {
        response = paths.at(path)(http, db);
    } else {
        return Http::response(404, "No such table");
    }

	return Http::response(response.status, get_resp_body(response), headers);
}
