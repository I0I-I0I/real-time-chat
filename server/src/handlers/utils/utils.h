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
    std::string extantion;
};

bool includes(const std::vector<std::string>& arr, const std::string& str);
std::string create_resp_body(const DBResponseStruct& response);
HttpResponseStruct get_resp_for_file(const HttpRequestStruct& http, HttpHeadersStruct& headers);
GetFileStruct get_file_form_frontend(std::string path);
std::map<std::string, std::string> get_headers_of_extantion(const std::string& extantion);
