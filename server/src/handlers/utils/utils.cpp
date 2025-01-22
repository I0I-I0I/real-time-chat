#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "../../config.h"
#include "../../http/http.h"
#include "../../db/db.h"
#include "./utils.h"

bool includes(const std::vector<std::string>& arr, const std::string& str) {
    for (const auto& item : arr)
        if (item == str) return true;
    return false;
}

std::string get_resp_body(const DBResponseStruct& response) {
    json body = {
        { "status", response.body.status },
        { "data", response.body.data },
        { "message", response.body.msg },
    };
    return body.dump();
}

std::map<std::string, std::string> get_headers_of_extantion(const std::string& extantion) {
    std::vector<std::string> image_types = {
        "png",
        "jpg",
        "jpeg",
        "gif",
        "svg",
        "webp",
        "ico",
        "avif",
    };
    std::map<std::string, std::string> headers;

    if (extantion == "html") {
        headers["content-type"] = "text/html";
        headers["x-content-type-options"] = "nosniff";
    } else if (extantion == "js"){
        headers["content-type"] = "text/javascript";
        headers["x-content-type-options"] = "script";
    } else if (extantion == "css") {
        headers["content-type"] = "text/css";
        headers["x-content-type-options"] = "style";
    } else if (includes(image_types, extantion)) {
        headers["content-type"] = "image/" + extantion;
    }

    return headers;
}

GetFileStruct get_file_form_frontend(std::string path) {
    std::ifstream file(FRONTED_PATH + path);

    if (!file.is_open()) {
        if (SPA_MODE) {
            path = "/index.html";
            file = std::ifstream(std::string(FRONTED_PATH) + path);
        } else {
            return { "File not found", path, "ERROR" };
        }
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return {
        buffer.str(),
        path,
        path.substr(path.find_last_of(".") + 1)
    };
}

HttpResponseStruct get_resp_for_file(const HttpRequestStruct& http, HttpHeadersStruct& headers) {
    std::string file_path;
    if (http.url.path.at(http.url.path.size() - 1).find('.') == std::string::npos) {
        file_path = "/index.html";
    } else {
        for (auto const& item : http.url.path) {
            file_path += item;
        }
    }

    GetFileStruct file = get_file_form_frontend(file_path);

    std::map<std::string, std::string> headers_of_ext = get_headers_of_extantion(file.extantion);
    for (const auto& [key, value] : headers_of_ext) {
        headers[key] = value;
    }

    return Http::response(200, file.body, headers);
}
