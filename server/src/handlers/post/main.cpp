#include <string>
#include "json.hpp"
#include "../../http/http.h"
#include "../../db/db.h"
#include "../../config.h"
#include "../handlers.h"
#include "../utils/utils.h"
#include "./post.h"

using json = nlohmann::json;

OnUrlFuncsList paths_post = {
    { "/users", on_users_post },
    { "/chats", on_chats_post },
    { "/messages", on_messages_post },
    { "/search", on_search_post }
};

HttpResponseStruct HandlerOn::post(const HttpRequestStruct& http) {
    HttpResponseStruct resp = validate_post(http);
    if (resp.status != "OK")
        return resp;

    std::string table = http.url.path[2];
    if (paths_post.find(table) == paths_post.end())
        return Http::response(StatusCode::not_found, "No such table");

    HttpHeadersStruct headers = {};
    headers["content-type"] = "application/json";

    DB db(DB_PATH);

    return paths_post.at(table)(http, db, headers);
}
