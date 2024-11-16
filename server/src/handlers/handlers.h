#pragma once

#include <any>
#include <string>
#include "../http/http.h"

class HandlerOn {
public:
	static std::string get(const HttpRequestStruct& data);
	static std::string post(const HttpRequestStruct& data);
	static std::string put(const HttpRequestStruct& data);
	static std::string del(const HttpRequestStruct& data);
};
