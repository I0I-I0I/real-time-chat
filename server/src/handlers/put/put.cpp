#include <string>
#include "../../http/http.h"
#include "../../db/db.h"
#include "./put.h"

HttpResponseStruct validate_put(const HttpRequestStruct& http) {
    if (http.url.path.at(0) != "/api")
        return Http::response(StatusCode::bad_request, "You missed '/api'");
    if (http.url.path.size() < 3)
        return Http::response(StatusCode::bad_request, "You missed table name or something");
    if (http.headers.find("content-type") == http.url.params.end())
        return Http::response(StatusCode::bad_request, "Missing Content-Type");
    if (http.headers.at("content-type") != "application/json")
        return Http::response(StatusCode::bad_request, "Unknown Content-Type, you can only pass 'application/json'");
    if (!json::accept(http.body))
        return Http::response(StatusCode::bad_request, "Not valid json");
    return { .status = "OK", .headers = {}, .body = "" };
}

HttpResponseStruct on_chats_put(const HttpRequestStruct& http, DB& db, HttpHeadersStruct& headers) {
    (void)db;
    (void)http;
    (void)headers;
    return Http::response(StatusCode::method_not_allowed, "Method not implemented");
}
