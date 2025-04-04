#pragma once

#include "../../http/http.h"
#include "../../db/db.h"

HttpResponseStruct on_messages_get(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers);
HttpResponseStruct on_users_get(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers);
HttpResponseStruct on_chats_get(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers);
HttpResponseStruct on_file_get(const HttpRequestStruct& http, HttpHeadersStruct headers = {});
HttpResponseStruct validate_get(const HttpRequestStruct& http);
HttpResponseStruct on_search_get(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers);
