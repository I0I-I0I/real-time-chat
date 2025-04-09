#include <experimental/filesystem>
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

std::string create_resp_body(const DBResponseStruct& response) {
    int status = StatusCodeMap.at(response.status).code;
    json body = {
        { "status", status },
        { "data", response.body.data },
        { "message", response.body.msg },
    };
    return body.dump();
}

std::map<std::string, std::string> get_headers_of_extantion(const std::string& path) {
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

    if (path.ends_with(".html")) {
        headers["content-type"] = "text/html";
        headers["x-content-type-options"] = "nosniff";
    } else if (path.ends_with(".js") || path.ends_with(".mjs") || path.ends_with(".cjs")){
        headers["content-type"] = "text/javascript";
        headers["x-content-type-options"] = "script";
    } else if (path.ends_with(".css")) {
        headers["content-type"] = "text/css";
        headers["x-content-type-options"] = "style";
    } else if (includes(image_types, path.substr(path.find_last_of(".") + 1))) {
        headers["content-type"] = "image/" + path.substr(path.find_last_of(".") + 1);
    }

    return headers;
}

std::string get_file_path(const HttpRequestStruct& http) {
    std::string file_path;
    if (http.url.path.at(http.url.path.size() - 1).find('.') == std::string::npos) {
        file_path = "/index.html";
    } else {
        for (auto const& item : http.url.path) {
            file_path += item;
        }
    }
    return file_path;
}

GetFileStruct get_file(const HttpRequestStruct& http) {
    std::string path = get_file_path(http);
    std::ifstream file(FRONTED_PATH + path);

    if (!file.is_open()) {
        if (SPA_MODE) {
            path = "/index.html";
            file = std::ifstream(std::string(FRONTED_PATH) + path);
        } else {
            return { "File not found", path };
        }
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return {
        buffer.str(),
        path
    };
}
