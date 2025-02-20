#include <string>
#include "../../http/http.h"
#include "../../db/db.h"
#include "../../../lib/json.hpp"
#include "../../config.h"
#include "../handlers.h"

using json = nlohmann::json;

HttpResponseStruct HandlerOn::del(const HttpRequestStruct& http) {
    HttpHeadersStruct headers = {
        { "connection", "close" }
    };

    if (http.url.path.at(0) != "/api") return Http::response(400, "You missed '/api'", headers);
    if (http.url.path.size() < 3) return Http::response(400, "You missed table name or something", headers);
    if (http.url.params.find("id") == http.url.params.end()) return Http::response(400, "Missing 'id'", headers);

    headers["content-type"] = "application/json";

    DB db(DB_PATH);

    std::string id = http.url.params.at("id");
    std::string table = http.url.path[2];
    DBResponseStruct response = db.delete_data(table, id);

    json body = {
        { "status", response.body.status },
        { "data", response.body.data },
        { "message", response.body.msg },
    };

    return Http::response(response.status, body.dump(), headers);
}
