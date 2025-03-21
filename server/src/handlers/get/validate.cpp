#include <string>
#include "../../http/http.h"
#include "./get.h"

HttpResponseStruct validate_get(const HttpRequestStruct& http) {
    if (http.url.path.size() < 3) return Http::response(StatusCode::bad_request, "You missed table name or something");
    return { .status = "OK", .headers = {}, .body = "" };
}
