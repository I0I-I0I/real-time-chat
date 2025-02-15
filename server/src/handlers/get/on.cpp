#include <string>
#include "../../http/http.h"
#include "../../db/db.h"
#include "./get.h"
#include "../utils/utils.h"

HttpResponseStruct on_file(const HttpRequestStruct& http, HttpHeadersStruct& headers) {
    if ((http.headers.find("connection") != http.headers.end())
            && (http.headers.at("connection") == "keep-alive")) {
        headers["connection"] = "keep-alive";
        headers["keep-alive"] = "timeout=15, max=100";
    } else {
        headers["connection"] = "close";
    }
    return get_resp_for_file(http, headers);
}

DBResponseStruct on_users(const HttpRequestStruct& http, DB& db) {
    std::string table = "users";
    DBResponseStruct response;
    if (http.url.params.find("login") != http.url.params.end()) {
        response = db.get_data_by("login", table, http.url.params.at("login"), { "id", "login", "username", "created_at" });
    } else if (http.url.params.find("id") != http.url.params.end()) {
        response = db.get_data_by("id", table, http.url.params.at("id"));
    } else {
        response = db.get_data(table);
    }
    return response;
}

DBResponseStruct on_chats(const HttpRequestStruct& http, DB& db) {
    std::string table = "chats";
    DBResponseStruct response;
    if (http.url.params.find("id") != http.url.params.end()) {
        response = db.get_data_by("id", table, http.url.params.at("id"));
    } else {
        response = db.get_data(table);
    }
    return response;
}
