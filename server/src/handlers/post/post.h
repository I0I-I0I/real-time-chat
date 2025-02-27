#pragma once

#include "../../http/http.h"
#include "../../db/db.h"

HttpResponseStruct on_messages_post(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers);
HttpResponseStruct on_users_post(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers);
HttpResponseStruct on_chats_post(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers);
HttpResponseStruct validate_post(const HttpRequestStruct& http);
