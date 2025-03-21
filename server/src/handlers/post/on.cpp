#include "../../http/http.h"
#include "../../db/db.h"
#include "../utils/utils.h"
#include <iostream>
#include <string>

HttpResponseStruct on_messages_post(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers) {
    std::string table = "messages";
    DBDataListStruct data = json::parse(http.body);
    DBResponseStruct response = db.insert_data(table, data, true);

    return Http::response(response.status, create_resp_body(response), headers);
}

HttpResponseStruct on_users_post(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers) {
    std::string table = "users";
    DBResponseStruct response;
    DBDataListStruct data = json::parse(http.body);

    if ((http.url.params.find("type") != http.url.params.end())
        && (http.url.params.at("type") == "check")) {
        response = db.check_password(table, data[0]["login"], data[0]["password"], {
            "id",
            "login",
            "username",
            "createdAt"
        });
        HttpResponseStruct resp = Http::response(response.status, create_resp_body(response), headers);
        return resp;
    }

    response = db.insert_data(table, data);
    return Http::response(response.status, create_resp_body(response), headers);
}

HttpResponseStruct on_chats_post(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers) {
    std::string table = "chats";
    std::string tableParticipants = "chatParticipants";
    DBDataListStruct data = json::parse(http.body);

    DBDataListStruct *dataParticipants = new DBDataListStruct;

    std::cout << "HI!" << std::endl;
    for (const auto& item : data) {
        for (auto it = item.begin(); it != item.end(); ++it) {
            dataParticipants->push_back(item);
            std::cout << item << std::endl;
        }

        // { "chatId", data.at("chatId") },
        // { "userId", "userId" },
    }

    db.insert_data(tableParticipants, *dataParticipants);

    free(dataParticipants);

    DBResponseStruct response = db.insert_data(table, data);

    return Http::response(response.status, create_resp_body(response), headers);
}
