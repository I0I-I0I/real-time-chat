#include <string>
#include "json.hpp"
#include "../../http/http.h"
#include "../../db/db.h"
#include "../../config.h"
#include "../handlers.h"
#include "../utils/utils.h"
#include "./put.h"

using json = nlohmann::json;

OnUrlFuncsList paths_put = {
    { "/chats", on_chats_put },
};

HttpResponseStruct HandlerOn::put(const HttpRequestStruct& http) {
    HttpResponseStruct resp = validate_put(http);
    if (resp.status != "OK")
        return resp;

    std::string table = http.url.path[2];
    if (paths_put.find(table) == paths_put.end())
        return Http::response(StatusCode::not_found, "No such table");

    HttpHeadersStruct headers = {
        { "content-type", "application/json" }
    };

    std::string dbPath = getDbPath();
    DB db(dbPath);

    return paths_put.at(table)(http, db, headers);
}
