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
std::string create_resp_body(const DBResponseStruct& response);
GetFileStruct get_file(const HttpRequestStruct& http);
std::map<std::string, std::string> get_headers_of_extantion(const std::string& path);
std::string get_file_path(const HttpRequestStruct& http);
