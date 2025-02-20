#pragma once

#include "../../http/http.h"
#include "../../db/db.h"

HttpResponseStruct on_messages(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers);
HttpResponseStruct on_users(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers);
HttpResponseStruct on_chats(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers);
HttpResponseStruct on_file(const HttpRequestStruct& http, HttpHeadersStruct headers = {});
