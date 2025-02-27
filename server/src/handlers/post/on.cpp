#include "../../http/http.h"
#include "../../db/db.h"
#include "../utils/utils.h"

HttpResponseStruct on_messages_post(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers) {
    std::string table = "messages";
    DBDataListStruct data = json::parse(http.body);
    DBResponseStruct response = db.insert_data(table, data);

    return Http::response(response.status, create_resp_body(response), headers);
}

HttpResponseStruct on_users_post(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers) {
    std::string table = "users";
    DBResponseStruct response;
    DBDataListStruct data = json::parse(http.body);

    if ((http.url.params.find("type") != http.url.params.end())
            && (http.url.params.at("type") == "check")) {
        response = db.check_password(table, data[0]["login"], data[0]["password"]);
        return Http::response(response.status, create_resp_body(response), headers);
    }

    response = db.insert_data(table, data);
    return Http::response(response.status, create_resp_body(response), headers);
}

HttpResponseStruct on_chats_post(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers) {
    std::string table = "chats";
    DBDataListStruct data = json::parse(http.body);
    DBResponseStruct response = db.insert_data(table, data);

    return Http::response(response.status, create_resp_body(response), headers);
}
