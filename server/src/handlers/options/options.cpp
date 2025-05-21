#include <string>
#include "../../http/http.h"
#include "../handlers.h"

HttpResponseStruct HandlerOn::options(const HttpRequestStruct&) {
    return Http::response(StatusCode::ok, "", {
        { "access-control-allow-methods", "GET, POST, PUT, DELETE, PATCH, OPTIONS" },
        { "access-control-allow-headers", "content-type, authorization" },
        { "connection", "keep-alive" },
        { "keep-alive", "timeout=15, max=100" }
    });
}
