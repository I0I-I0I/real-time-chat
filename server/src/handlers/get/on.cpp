#include <string>
#include "../../http/http.h"
#include "../../db/db.h"
#include "../utils/utils.h"
#include "./get.h"

HttpResponseStruct on_file_get(const HttpRequestStruct& http, HttpHeadersStruct headers) {
    if ((http.headers.find("connection") != http.headers.end())
            && (http.headers.at("connection") == "keep-alive")) {
        headers["connection"] = "keep-alive";
        headers["keep-alive"] = "timeout=15, max=100";
    } else {
        headers["connection"] = "close";
    }

    GetFileStruct file = get_file(http);
    if (file.body == "File not found") return Http::response(StatusCode::not_found, "File not found");

    std::map<std::string, std::string> headers_of_ext = get_headers_of_extantion(file.path);
    for (const auto& [key, value] : headers_of_ext) {
        headers[key] = value;
    }

    return Http::response(StatusCode::ok, file.body, headers);
}

HttpResponseStruct on_users_get(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers) {
    std::string table = "users";
    DBResponseStruct response;
    if (http.url.params.find("login") != http.url.params.end()) {
        response = db.get_data_by(
            "login",
            table,
            http.url.params.at("login"),
            { "id", "login", "username", "createdAt" }
        );
    } else if (http.url.params.find("id") != http.url.params.end()) {
        response = db.get_data_by("id", table, http.url.params.at("id"));
    } else {
        response = db.get_data(table);
    }
    return Http::response(response.status, create_resp_body(response), headers);
}

HttpResponseStruct on_chats_get(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers) {
    std::string table = "chats";
    DBResponseStruct response;
    if (http.url.params.find("id") != http.url.params.end()) {
        response = db.get_data_by("id", table, http.url.params.at("id"));
    } else {
        response = db.get_data(table);
    }
    return Http::response(response.status, create_resp_body(response), headers);
}

HttpResponseStruct on_messages_get(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers) {
    DBResponseStruct response;
    std::string table = "messages";
    if (http.url.params.find("chatId") == http.url.params.end()) {
        return Http::response(StatusCode::bad_request, "Missing 'chatId'");
    }
    response = db.get_data_by("chatId", table, http.url.params.at("chatId"));
    return Http::response(response.status, create_resp_body(response), headers);
}
