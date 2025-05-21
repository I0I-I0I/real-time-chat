#include "../../http/http.h"
#include "../../db/db.h"
#include <string>

HttpResponseStruct validate_del(const HttpRequestStruct& http) {
    if (http.url.path.at(0) != "/api")
        return Http::response(StatusCode::bad_request, "You missed '/api'");
    if (http.url.path.size() < 3)
        return Http::response(StatusCode::bad_request, "You missed table name or something");
    if (http.url.params.find("id") == http.url.params.end())
        return Http::response(StatusCode::bad_request, "Missing 'id'");
    return { .status = "OK", .headers = {}, .body = "" };
}

HttpResponseStruct on_del_users(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers) {
    std::string table = "users";
    std::string id = http.url.params.at("id");
    ResponseDataStruct response = db.delete_data_by("id", table, id);

    return Http::response(response, headers);
}

HttpResponseStruct on_del_chats(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers) {
    std::string table = "chats";
    std::string table_participants = "chatParticipants";
    std::string table_messages = "messages";
    std::string id = http.url.params.at("id");

    ResponseDataStruct response = db.delete_data_by("id", table, id);
    if (response.status == StatusCode::ok) {
        db.delete_data_by("chatId", table_participants, id);
        db.delete_data_by("chatId", table_messages, id);
    }

    return Http::response(response, headers);
}

HttpResponseStruct on_del_messages(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers) {
    std::string table = "messages";
    std::string id = http.url.params.at("id");

    ResponseDataStruct response = db.delete_data_by("id", table, id);

    return Http::response(response, headers);
}
