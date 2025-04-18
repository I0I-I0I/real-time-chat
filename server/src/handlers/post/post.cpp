#include "../../http/http.h"
#include "../../db/db.h"
#include "../utils/utils.h"
#include "./post.h"
#include <string>

using json = nlohmann::json;

/**
 * Validation
 */

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

/**
 * Auth
 */

HttpResponseStruct on_register_post(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers) {
    std::string table = "users";
    DBDataListStruct all_data = json::parse(http.body);
    DBDataStruct data = all_data[0];

    if (!data.contains("login"))
        return Http::response(StatusCode::bad_request, "Missing 'login'");
    if (!data.contains("password"))
        return Http::response(StatusCode::bad_request, "Missing 'password'");
    if (!data.contains("username"))
        return Http::response(StatusCode::bad_request, "Missing 'username'");

    const std::string salt = Encode::salt(16);
    const std::string password_hash = Encode::encode(data.at("password"), salt);

    Sessions sessions(db);
    SessionItem session_item = sessions.create(data.at("login"), password_hash);

    all_data[0].at("password") = password_hash;
    all_data[0].push_back({"salt", salt});

    ResponseDataStruct db_response = db.insert_data(table, all_data, ExecuteType::get);
    if (db_response.status == StatusCode::ok)  {
        db_response.data[0]["hash"] = session_item.hash;
    } else {
        sessions.remove(data.at("login"));
    }

    return Http::response(db_response, headers);
}

HttpResponseStruct on_login_post(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers) {
    std::string table = "users";
    DBDataStruct data = json::parse(http.body)[0];

    Sessions sessions(db);

    if (data.contains("hash")) {
        CheckSessionStruct status = sessions.check(data.at("hash"));
        if (status.status == SessionCheckStatus::BAD_USER)
            return Http::response(StatusCode::bad_request, "Bad login");
        if (status.status != SessionCheckStatus::OK)
            return Http::response(StatusCode::bad_request, "Bad password");
        ResponseDataStruct db_response = db.get_data_by("login", table, status.login);
        db_response.data[0]["hash"] = data.at("hash");
        return Http::response(db_response, headers);
    }

    if (!data.contains("login"))
        return Http::response(StatusCode::bad_request, "Missing 'login'");
    if (!data.contains("password"))
        return Http::response(StatusCode::bad_request, "Missing 'password'");

    ResponseDataStruct db_response;
    std::string login = data.at("login");
    db_response = db.get_data_by("login", table, login);
    if (db_response.status != StatusCode::ok) {
        return Http::response(StatusCode::not_found, "User with this login does not exitsts");
    }

    std::string encoded_password = Encode::encode(data.at("password"), db_response.data[0].at("salt"));

    db_response = db.check_password(
        table,
        data.at("login"),
        encoded_password,
        { "id", "login", "username", "createdAt" }
    );

    SessionItem session_item = sessions.create(data.at("login"), data.at("password"));
    if (db_response.status == StatusCode::ok)  {
        db_response.data[0]["hash"] = session_item.hash;
    } else {
        sessions.remove(data.at("login"));
    }

    return Http::response(db_response, headers);
}

HttpResponseStruct on_logout_post(const HttpRequestStruct& http, DB& db, HttpHeadersStruct&) {
    DBDataStruct data = json::parse(http.body)[0];

    if (!data.contains("login"))
        return Http::response(StatusCode::bad_request, "Missing 'login'");
    if (!data.contains("hash"))
        return Http::response(StatusCode::bad_request, "Missing 'hash'");

    Sessions sessions(db);
    int status = sessions.remove(data.at("login"));
    if (status == -2)
        return Http::response(StatusCode::bad_request, "Wrong hash");
    if (status != 0)
        return Http::response(StatusCode::bad_request, "Session not found");
    return Http::response(StatusCode::ok, "Session successfully closed");
}

/**
 * Other
 */

HttpResponseStruct on_messages_post(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers) {
    std::string table = "messages";
    std::string chat_table = "chats";

    DBDataListStruct data = json::parse(http.body);
    ResponseDataStruct db_response = db.insert_data(table, data, ExecuteType::get);
    if (db_response.status == StatusCode::ok) {
        DBDataStruct data = db_response.data[0];
        DBDataStruct inserted_data = {
            {"lastMessageId", std::to_string((int)data["id"])},
        };
        db.update_data(chat_table, std::to_string((int)data["chatId"]), inserted_data);
    }

    return Http::response(db_response, headers);
}

HttpResponseStruct on_users_post(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers) {
    std::string table = "users";
    DBDataListStruct data = json::parse(http.body);
    ResponseDataStruct db_response = db.insert_data(table, data);
    return Http::response(db_response, headers);
}

HttpResponseStruct on_participants_post(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers) {
    std::string table = "chatParticipants";
    DBDataListStruct data = json::parse(http.body);
    ResponseDataStruct db_response = db.insert_data(table, data);
    return Http::response(db_response, headers);
}

HttpResponseStruct on_chats_post(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers) {
    if (http.url.params.find("type") != http.url.params.end()
            && http.url.params.at("type") == "addParticipants")
        return on_participants_post(http, db, headers);

    std::string table = "chats";
    DBDataListStruct data = json::parse(http.body);
    ResponseDataStruct db_response = db.insert_data(table, data, ExecuteType::get);

    const int& chat_id = db_response.data[0]["id"];
    if (http.url.params.find("userId") == http.url.params.end()) {
        db.delete_data_by("id", "chats", std::to_string(chat_id));
        return Http::response(StatusCode::bad_request, "Missing 'userId'");
    }

    const std::string& user_id = http.url.params.at("userId");
    ResponseDataStruct res = db.get_data_by("id", "users", user_id);
    if (res.status != StatusCode::ok) {
        db.delete_data_by("id", "chats", std::to_string(chat_id));
        return Http::response(res, headers);
    }

    DBDataListStruct participants_data = {{
        { "userId", user_id },
        { "chatId", std::to_string(chat_id) }
    }};
    db.insert_data("chatParticipants", participants_data);

    return Http::response(db_response, headers);
}
