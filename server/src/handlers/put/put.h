#pragma once

#include "../../http/http.h"
#include "../../db/db.h"

HttpResponseStruct validate_put(const HttpRequestStruct& http);
HttpResponseStruct on_chats_put(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers);
