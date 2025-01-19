#pragma once

#include <any>
#include <functional>
#include <string>
#include "../http/http.h"

using HandlerOnFunc = std::function<HttpResponseStruct(const HttpRequestStruct&)>;

class HandlerOn {
public:
    static HttpResponseStruct get(const HttpRequestStruct& data);
    static HttpResponseStruct post(const HttpRequestStruct& data);
    static HttpResponseStruct put(const HttpRequestStruct& data);
    static HttpResponseStruct del(const HttpRequestStruct& data);
    static HttpResponseStruct options(const HttpRequestStruct& data);
};
