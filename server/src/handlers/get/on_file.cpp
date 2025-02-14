#include "../../config.h"
#include "../../http/http.h"
#include "../utils/utils.h"

HttpResponseStruct on_file(const HttpRequestStruct& http, HttpHeadersStruct& headers) {
    if ((http.headers.find("connection") != http.headers.end())
            && (http.headers.at("connection") == "keep-alive")) {
        headers["connection"] = "keep-alive";
        headers["keep-alive"] = "timeout=15, max=100";
    } else {
        headers["connection"] = "close";
    }
    return get_resp_for_file(http, headers);
}
