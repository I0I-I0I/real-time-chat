#pragma once

#include <string>
#include <vector>
#include "../../http/http.h"
#include "../../db/db.h"

using OnUrlFunc = std::function<HttpResponseStruct(const HttpRequestStruct&, DB&, HttpHeadersStruct&)>;
using OnUrlFuncsList = std::map<std::string, OnUrlFunc>;

struct GetFileStruct {
    std::string body;
    std::string path;
};

bool includes(const std::vector<std::string>& arr, const std::string& str);
std::string create_resp_body(const ResponseDataStruct& response);
GetFileStruct get_file(const HttpRequestStruct& http);
std::map<std::string, std::string> get_headers_of_extantion(const std::string& path);
std::string get_file_path(const HttpRequestStruct& http);

/**
 * Session managment
 */

struct SessionItem {
    std::string salt;
    std::string hash;
};

enum SessionCheckStatus {
    OK,
    BAD_USER,
    BAD_PASSWORD
};

struct CheckSessionStruct {
    SessionCheckStatus status;
    std::string login;
};

class Sessions {
private:
    DB &db;
    const std::string db_table = "sessions";
    const std::string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    std::string generate_salt(int length);
    std::string encode_value(const std::string& value, const std::string& salt);

public:
    Sessions(DB &db_)
        : db(db_) {}

    SessionItem create(const std::string login, std::string password);
    int remove(const std::string login);
    int remove(const std::string login, const std::string hash);
    CheckSessionStruct check(std::string hash);

};
