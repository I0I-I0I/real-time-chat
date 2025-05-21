#include <string>
#include "json.hpp"
#include "../../http/http.h"
#include "../../db/db.h"
#include "../../config.h"
#include "../utils/utils.h"
#include "../handlers.h"
#include "./get.h"

using json = nlohmann::json;

OnUrlFuncsList paths_get = {
    { "/users", on_users_get },
    { "/chats", on_chats_get },
    { "/messages", on_messages_get },
    { "/search", on_search_get }
};

HttpResponseStruct HandlerOn::get(const HttpRequestStruct& http) {
    if (http.url.path.at(0) != "/api") return on_file_get(http);

    HttpResponseStruct resp = validate_get(http);
    if (resp.status != "OK")
        return resp;

    std::string table = http.url.path[2];
    if (paths_get.find(table) == paths_get.end())
        return Http::response(StatusCode::not_found, "No such table");

    HttpHeadersStruct headers = {
        { "content-type", "application/json" }
    };

    std::string dbPath = getDbPath();
    DB db(dbPath);

    return paths_get.at(table)(http, db, headers);
}
