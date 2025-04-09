#include "../../http/http.h"
#include "../../db/db.h"
#include "./get.h"

HttpResponseStruct handleGetChatById(
    const HttpRequestStruct& http,
    DB& db,
    const std::string& chatsTable,
    HttpHeadersStruct& headers
) {
    ResponseDataStruct db_response = db.get_data_by("id", chatsTable, http.url.params.at("id"));
    if (db_response.status != StatusCode::ok)
        return Http::response(db_response.status, "Chat not found");

    DBDataStruct& data = db_response.data[0];
    std::string lastMessageId = data.at("lastMessageId");
    DBDataStruct lastMessage = db.get_data_by("id", "messages", lastMessageId).data[0];
    data["lastMessage"] = lastMessage;

    return Http::response(db_response, headers);
}

HttpResponseStruct handleGetChatsByUserId(
    const HttpRequestStruct& http,
    DB& db,
    const std::string& participants_table,
    const std::string& chatsTable,
    HttpHeadersStruct& headers
) {
    ResponseDataStruct db_response = db.get_data_by("userId", participants_table, http.url.params.at("userId"), { "chatId" });
    if (db_response.status != StatusCode::ok)
        return Http::response(StatusCode::not_found, "No chats found for this user");

    DBDataListStruct chatIds = db_response.data;
    std::vector<std::string> ids;
    for (const DBDataStruct& item : chatIds) {
        ids.push_back(std::to_string((int)item.at("chatId")));
    }

    db_response = db.get_data_by("id", chatsTable, ids);
    if (db_response.status != StatusCode::ok)
        return Http::response(StatusCode::not_found, "No chats found for this user");

    for (DBDataStruct& item : db_response.data) {
        std::string last_message_id = std::to_string((int)item.at("lastMessageId"));
        ResponseDataStruct last_message_response = db.get_data_by("id", "messages", last_message_id);
        if (last_message_response.status == StatusCode::ok) {
            item["lastMessage"] = last_message_response.data[0];
            item.erase("lastMessageId");
        }
    }

    return Http::response(db_response, headers);
}
