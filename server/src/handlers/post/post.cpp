#include "../../http/http.h"
#include "../../db/db.h"
#include "../utils/utils.h"
#include <iostream>
#include <string>

using json = nlohmann::json;

HttpResponseStruct validate_post(const HttpRequestStruct& http) {
    if (http.url.path.at(0) != "/api")
        return Http::response(StatusCode::bad_request, "You missed '/api'");
    if (http.url.path.size() < 3)
        return Http::response(StatusCode::bad_request, "You missed table name or something");
    if (http.headers.find("content-type") == http.url.params.end())
        return Http::response(StatusCode::bad_request, "Missing Content-Type");
    if (http.headers.at("content-type") != "application/json")
        return Http::response(StatusCode::bad_request, "Unknown Content-Type, you can only pass 'application/json'");
    if (!json::accept(http.body))
        return Http::response(StatusCode::bad_request, "Not valid json");
    return { .status = "OK", .headers = {}, .body = "" };
}

HttpResponseStruct on_messages_post(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers) {
    std::string table = "messages";
    std::string chat_table = "chats";

    DBDataListStruct data = json::parse(http.body);
    DBResponseStruct response = db.insert_data(table, data, ExecuteType::get);
    if (response.status == StatusCode::ok) {
        DBDataStruct data = response.body.data[0];
        DBDataStruct inserted_data = {
            {"lastMessageId", std::to_string((int)data["id"])},
        };
        db.update_data(chat_table, std::to_string((int)data["chatId"]), inserted_data);
    }

    return Http::response(response.status, create_resp_body(response), headers);
}

HttpResponseStruct on_users_post(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers) {
    std::string table = "users";
    DBResponseStruct response;
    DBDataListStruct data = json::parse(http.body);

    if ((http.url.params.find("type") != http.url.params.end()) && (http.url.params.at("type") == "check")) {
        response = db.check_password(table, data[0]["login"], data[0]["password"], { "id", "login", "username", "createdAt" });
        HttpResponseStruct resp = Http::response(response.status, create_resp_body(response), headers);
        return resp;
    }

    response = db.insert_data(table, data);
    return Http::response(response.status, create_resp_body(response), headers);
}

HttpResponseStruct on_participants_post(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers) {
    std::string table = "chatParticipants";
    DBDataListStruct data = json::parse(http.body);
    DBResponseStruct response = db.insert_data(table, data);
    return Http::response(response.status, create_resp_body(response), headers);
}

HttpResponseStruct on_chats_post(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers) {
    if (http.url.params.find("type") != http.url.params.end()
            && http.url.params.at("type") == "addParticipants")
        return on_participants_post(http, db, headers);

    std::string table = "chats";
    DBDataListStruct data = json::parse(http.body);
    DBResponseStruct response = db.insert_data(table, data, ExecuteType::get);

    const int& chat_id = response.body.data[0]["id"];
    if (http.url.params.find("userId") == http.url.params.end()) {
        db.delete_data_by("id", "chats", std::to_string(chat_id));
        return Http::response(StatusCode::bad_request, "Missing 'userId'");
    }

    const std::string& user_id = http.url.params.at("userId");
    DBResponseStruct res = db.get_data_by("id", "users", user_id);
    if (res.status != StatusCode::ok) {
        db.delete_data_by("id", "chats", std::to_string(chat_id));
        return Http::response(res.status, create_resp_body(res), headers);
    }

    DBDataListStruct participants_data = {{
        { "userId", user_id },
        { "chatId", std::to_string(chat_id) }
    }};
    db.insert_data("chatParticipants", participants_data);

    return Http::response(response.status, create_resp_body(response), headers);
}
