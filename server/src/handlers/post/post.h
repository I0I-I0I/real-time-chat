#pragma once

#include "../../http/http.h"
#include "../../db/db.h"

HttpResponseStruct validate_post(const HttpRequestStruct& http);

HttpResponseStruct on_messages_post(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers);
HttpResponseStruct on_users_post(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers);
HttpResponseStruct on_chats_post(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers);
HttpResponseStruct on_participants_post(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers);

HttpResponseStruct on_logout_post(const HttpRequestStruct& http, DB&, HttpHeadersStruct&);
HttpResponseStruct on_login_post(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers);
HttpResponseStruct on_register_post(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers);
