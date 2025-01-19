#pragma once

#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "../../http/http.h"
#include "../../db/db.h"

struct GetFileStruct {
	std::string body;
	std::string path;
	std::string extantion;
};

bool includes(const std::vector<std::string>& arr, const std::string& str);
std::map<std::string, std::string> get_headers_of_extantion(const std::string& extantion);
GetFileStruct get_file(const std::string& path);
std::string get_resp_for_file(const HttpRequestStruct& http, HttpHeadersStruct& headers);
std::string get_resp_body(const DBResponseStruct& response);
