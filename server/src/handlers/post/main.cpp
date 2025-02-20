#include <string>
#include "../../../lib/json.hpp"
#include "../../http/http.h"
#include "../../db/db.h"
#include "../../config.h"
#include "../handlers.h"
#include "../utils/utils.h"

using json = nlohmann::json;

HttpResponseStruct HandlerOn::post(const HttpRequestStruct& http) {
    HttpHeadersStruct headers = {
        { "connection", "close" }
    };

    if (http.url.path.at(0) != "/api") return Http::response(400, "You missed '/api'", headers);
    if (http.url.path.size() < 3) return Http::response(400, "You missed table name or something", headers);
    if (http.headers.find("content-type") == http.url.params.end()) return Http::response(400, "Missing Content-Type", headers);
    if (http.headers.at("content-type") != "application/json") return Http::response(400, "Unknown Content-Type, you can only pass 'application/json'", headers);
    if (!json::accept(http.body)) return Http::response(400, "Not valid json", headers);

    headers["content-type"] = "application/json";

    DB db(DB_PATH);
    std::string table = http.url.path[2].substr(1);
    DBDataListStruct data = json::parse(http.body);
    DBResponseStruct response;

    if ((http.url.params.find("type") != http.url.params.end()) && (http.url.params.at("type") == "check")) {
        response = db.check_password(table, data[0]["login"], data[0]["password"], { "id", "login", "username", "created_at" });
    } else {
        response = db.insert_data(table, data);
    }

    return Http::response(response.status, create_resp_body(response), headers);
}
