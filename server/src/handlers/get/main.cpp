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

OnUrlFuncsList paths = {
    { "/users", on_users },
    { "/chats", on_chats },
    { "/messages", on_messages }
};

HttpResponseStruct HandlerOn::get(const HttpRequestStruct& http) {
    HttpHeadersStruct headers = {
        { "connection", "close" }
    };

    if (http.url.path.at(0) != "/api") return on_file(http, headers);
    if (http.url.path.size() < 3) return Http::response(400, "You missed table name or something", headers);

    std::string table = http.url.path[2];
    if (paths.find(table) == paths.end()) return Http::response(404, "No such table", headers);

    DB db(DB_PATH);
    headers["content-type"] = "application/json";

    return paths.at(table)(http, db, headers);
}
