#include <string>
#include "../../http/http.h"
#include "../../db/db.h"
#include "../../../lib/json.hpp"
#include "../../config.h"
#include "../handlers.h"

using json = nlohmann::json;

HttpResponseStruct HandlerOn::put(const HttpRequestStruct& http) {
    HttpHeadersStruct headers = {
        { "connection", "close" }
    };

    if (http.url.path.at(0) != "/api")
        return Http::response(400, "You missed '/api'", headers);

    if (http.url.path.size() < 3)
        return Http::response(400, "You missed table name or something", headers);

    if (http.headers.find("content-type") == http.url.params.end())
        return Http::response(400, "Missing Content-Type", headers);

    if (http.headers.at("content-type") != "application/json")
        return Http::response(400, "Unknown Content-Type, you can only pass 'application/json'", headers);

    DB db(DB_PATH);

    if (http.url.params.find("id") == http.url.params.end())
        return Http::response(400, "Missing 'id'", headers);
    std::string id = http.url.params.at("id");

    std::string table = http.url.path[2];

    if (http.headers.at("content-type") != "application/json")
        return Http::response(400, "Unknown Content-Type");

    if (!json::accept(http.body))
        return Http::response(400, "Not valid json");

    headers["content-type"] = "application/json";

    DBDataListStruct recv_body = json::parse(http.body);
    DBResponseStruct response = db.update_data(table, id, recv_body[0]);

    json body = {
        { "status", response.body.status },
        { "data", response.body.data },
        { "message", response.body.msg },
    };

    return Http::response(response.status, body.dump(), headers);
}
