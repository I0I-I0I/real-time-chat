#include <string>
#include "../../http/http.h"
#include "../../db/db.h"
#include "../utils/utils.h"
#include "./get.h"

HttpResponseStruct validate_get(const HttpRequestStruct& http) {
    if (http.url.path.size() < 3)
        return Http::response(StatusCode::bad_request, "You missed table name or something");
    return { .status = "OK", .headers = {}, .body = "" };
}

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
    ResponseDataStruct db_response;

    std::string by = "";
    if (http.url.params.find("login") != http.url.params.end()) {
        by = "login";
    } else if (http.url.params.find("id") != http.url.params.end()) {
        by = "id";
    } else {
        db_response = db.get_data( table, { "id", "login", "username", "createdAt" });
        return Http::response(db_response, headers);
    }

    db_response = db.get_data_by(
        by,
        table,
        http.url.params.at(by),
        { "id", "login", "username", "createdAt" }
    );

    return Http::response(db_response, headers);
}

HttpResponseStruct on_chats_get(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers) {
    const std::string chats_table = "chats";
    const std::string participants_table = "chatParticipants";

    if (http.url.params.find("id") != http.url.params.end())
        return handleGetChatById(http, db, chats_table, headers);
    if (http.url.params.find("userId") != http.url.params.end())
        return handleGetChatsByUserId(http, db, participants_table, chats_table, headers);

    return Http::response(StatusCode::bad_request, "Missing 'id' or 'userId'");
}

HttpResponseStruct on_messages_get(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers) {
    ResponseDataStruct db_response;
    std::string table = "messages";
    std::string by = "";
    std::string id = "";

    if (http.url.params.find("chatId") != http.url.params.end()) {
        by = "chatId";
        id = http.url.params.at("chatId");
    } else if (http.url.params.find("id") != http.url.params.end()) {
        by = "id";
        id = http.url.params.at("id");
    } else {
        return Http::response(StatusCode::bad_request, "Missing 'chatId' or 'id'");
    }

    db_response = db.get_data_by(by, table, id);
    return Http::response(db_response, headers);
}

HttpResponseStruct on_search_get(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers) {
    if (http.url.params.find("table") == http.url.params.end())
        return Http::response(StatusCode::bad_request, "Missing 'table'");
    if (http.url.params.find("by") == http.url.params.end())
        return Http::response(StatusCode::bad_request, "Missing 'by'");
    if (http.url.params.find("value") == http.url.params.end())
        return Http::response(StatusCode::bad_request, "Missing 'value'");

    std::string table = http.url.params.at("table");
    std::string by = http.url.params.at("by");
    std::string value = http.url.params.at("value");

    std::vector<std::string> fileds = {};
    if (table == "users")
        fileds = { "id", "login", "username", "createdAt" };

    ResponseDataStruct db_response = db.search_data(by, table, value, fileds);
    return Http::response(db_response, headers);
}
