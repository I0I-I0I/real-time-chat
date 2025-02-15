#pragma once

#include <string>
#include "../../http/http.h"
#include "../../db/db.h"

DBResponseStruct on_users(const HttpRequestStruct& http, DB& db);
DBResponseStruct on_chats(const HttpRequestStruct& http, DB& db);
HttpResponseStruct on_file(const HttpRequestStruct& http, HttpHeadersStruct& headers);
