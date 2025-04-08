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
    DBResponseStruct db_response;

    std::string by = "";
    if (http.url.params.find("login") != http.url.params.end()) {
        by = "login";
    } else if (http.url.params.find("id") != http.url.params.end()) {
        by = "id";
    }

    if (by == "") {
        db_response = db.get_data(table);
    } else {
        db_response = db.get_data_by(
            by,
            table,
            http.url.params.at(by),
            { "id", "login", "username", "createdAt" }
        );
    }

    return Http::response(db_response.status, create_resp_body(db_response), headers);
}

HttpResponseStruct on_chats_get(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers) {
    std::string table = "chats";
    DBResponseStruct db_response;
    if (http.url.params.find("id") != http.url.params.end()) {
        db_response = db.get_data_by("id", table, http.url.params.at("id"));
        DBDataStruct* data = &db_response.body.data[0];
        std::string lastMessageId = data->at("lastMessageId");
        json lastMessage = db.get_data_by("id", "messages", lastMessageId).body.data[0];
        data->at("lastMessage") = lastMessage;
    } else {
        db_response = db.get_data(table);
        DBDataListStruct* data = &db_response.body.data;
        for (DBDataStruct& item : *data) {
            int lastMessageId = item["lastMessageId"];
            DBResponseStruct d = db.get_data_by("id", "messages", std::to_string(lastMessageId));
            if (d.status != StatusCode::ok) continue;
            item["lastMessage"] = d.body.data[0];
            item.erase("lastMessageId");
        }
    }
    return Http::response(db_response.status, create_resp_body(db_response), headers);
}

HttpResponseStruct on_messages_get(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers) {
    DBResponseStruct db_response;
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
    return Http::response(db_response.status, create_resp_body(db_response), headers);
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

    DBResponseStruct db_response = db.search_data(by, table, value, { "id", "login", "username", "createdAt" });
    return Http::response(db_response.status, create_resp_body(db_response), headers);
}
