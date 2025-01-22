#pragma once

#include <string>
#include "../../http/http.h"
#include "../../db/db.h"

using OnUrlFunc = std::function<DBResponseStruct(const HttpRequestStruct&, DB&)>;

DBResponseStruct on_users(const HttpRequestStruct& http, DB& db);
HttpResponseStruct on_file(const HttpRequestStruct& http, HttpHeadersStruct& headers);
