#include <string>
#include "../../http/http.h"
#include "../utils/utils.h"
#include "../handlers.h"
#include "./post.h"

using json = nlohmann::json;

HttpResponseStruct validate_post(const HttpRequestStruct& http) {
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
