#include <string>
#include "json.hpp"
#include "../../http/http.h"
#include "../../db/db.h"
#include "../../config.h"
#include "../handlers.h"

using json = nlohmann::json;

HttpResponseStruct HandlerOn::del(const HttpRequestStruct& http) {
    if (http.url.path.at(0) != "/api") return Http::response(StatusCode::bad_request, "You missed '/api'");
    if (http.url.path.size() < 3) return Http::response(StatusCode::bad_request, "You missed table name or something");
    if (http.url.params.find("id") == http.url.params.end()) return Http::response(StatusCode::bad_request, "Missing 'id'");

    HttpHeadersStruct headers = {
        { "content-type", "application/json" }
    };

    DB db(DB_PATH);
    std::string id = http.url.params.at("id");
    std::string table = http.url.path[2];
    DBResponseStruct response = db.delete_data_by("id", table, id);

    json body = {
        { "status", response.body.status },
        { "data", response.body.data },
        { "message", response.body.msg },
    };

    return Http::response(response.status, body.dump(), headers);
}
