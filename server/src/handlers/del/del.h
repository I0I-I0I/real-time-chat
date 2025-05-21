#pragma once

#include "../../http/http.h"
#include "../../db/db.h"

HttpResponseStruct on_del_users(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers);
HttpResponseStruct on_del_chats(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers);
HttpResponseStruct on_del_messages(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers);
HttpResponseStruct validate_del(const HttpRequestStruct& http);
