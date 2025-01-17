#pragma once

#include <any>
#include <functional>
#include <string>
#include "../http/http.h"

using HandlerOnFunc = std::function<std::string(const HttpRequestStruct&)>;

class HandlerOn {
public:
    static std::string get(const HttpRequestStruct& data);
    static std::string post(const HttpRequestStruct& data);
    static std::string put(const HttpRequestStruct& data);
    static std::string del(const HttpRequestStruct& data);
    static std::string options(const HttpRequestStruct& data);
};
