#include <string>
#include "../../http/http.h"
#include "../../db/db.h"
#include "../../config.h"
#include "../utils/utils.h"
#include "../handlers.h"
#include "./del.h"

OnUrlFuncsList paths_del = {
    { "/users", on_del_users },
    { "/chats", on_del_chats },
    { "/messages", on_del_messages },
};

HttpResponseStruct HandlerOn::del(const HttpRequestStruct& http) {
    HttpResponseStruct resp = validate_del(http);
    if (resp.status != "OK")
        return resp;

    std::string table = http.url.path[2];
    if (paths_del.find(table) == paths_del.end())
        return Http::response(StatusCode::not_found, "No such table");

    HttpHeadersStruct headers = {};
    headers["content-type"] = "application/json";

    std::string dbPath = getDbPath();
    DB db(dbPath);

    return paths_del.at(table)(http, db, headers);
}
