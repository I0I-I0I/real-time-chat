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
    if (http.url.path.at(0) != "/api") return on_file(http);
    if (http.url.path.size() < 3) return Http::response(StatusCode::bad_request, "You missed table name or something");

    std::string table = http.url.path[2];
    if (paths.find(table) == paths.end()) return Http::response(StatusCode::not_found, "No such table");

    HttpHeadersStruct headers = {
        { "content-type", "application/json" }
    };

    DB db(DB_PATH);

    return paths.at(table)(http, db, headers);
}
