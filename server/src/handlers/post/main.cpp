#include <string>
#include "json.hpp"
#include "../../http/http.h"
#include "../../db/db.h"
#include "../../config.h"
#include "../handlers.h"
#include "../utils/utils.h"

using json = nlohmann::json;

HttpResponseStruct HandlerOn::post(const HttpRequestStruct& http) {
    if (http.url.path.at(0) != "/api") return Http::response(StatusCode::bad_request, "You missed '/api'");
    if (http.url.path.size() < 3) return Http::response(StatusCode::bad_request, "You missed table name or something");
    if (http.headers.find("content-type") == http.url.params.end()) return Http::response(StatusCode::bad_request, "Missing Content-Type");
    if (http.headers.at("content-type") != "application/json") return Http::response(StatusCode::bad_request, "Unknown Content-Type, you can only pass 'application/json'");
    if (!json::accept(http.body)) return Http::response(StatusCode::bad_request, "Not valid json");

    HttpHeadersStruct headers = {
        { "content-type", "application/json" }
    };

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
