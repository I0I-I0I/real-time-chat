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

std::map<std::string, OnUrlFunc> paths = {
    { "/users", on_users },
    { "/chats", on_chats },
};

HttpResponseStruct HandlerOn::get(const HttpRequestStruct& http) {
    HttpHeadersStruct headers = {
        { "connection", "close" }
    };

    if (http.url.path.at(0) != "/api") return on_file(http, headers);
    if (http.url.path.size() < 3) return Http::response(400, "Something missing in URL");

    headers["content-type"] = "application/json";

    DB db(DB_PATH);

    int status = 0;
    HandelPathRet create_response = handle_path(&status, paths, http.url.path[2]);

    if (status == -1) {
        return Http::response(404, "No such table");
    }
    return create_response(http, db, headers);
}
