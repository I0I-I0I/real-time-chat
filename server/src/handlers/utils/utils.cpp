#include <csignal>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <sstream>
#include <string>
#include <vector>
#include <openssl/sha.h>
#include "../../config.h"
#include "../../http/http.h"
#include "../../db/db.h"
#include "./utils.h"

bool includes(const std::vector<std::string>& arr, const std::string& str) {
    for (const auto& item : arr)
        if (item == str) return true;
    return false;
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
    std::ifstream file(std::string(getFrontendPath()) + path);

    if (!file.is_open()) {
        if (SPA_MODE) {
            path = "/index.html";
            file = std::ifstream(std::string(getFrontendPath()) + path);
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

/**
 * Session managment
 */

SessionItem Sessions::create(const std::string login, std::string password) {
    ResponseDataStruct db_response = this->db.get_data_by("login", this->db_table, login);
    if (db_response.status == StatusCode::ok) {
        return { "", db_response.data[0]["hash"] };
    }

    std::string salt = Encode::salt(16);
    std::string session_hash = Encode::encode(password, salt);
    DBDataStruct data = { { "login", login }, { "hash", session_hash } };
    DBDataListStruct data_list;
    data_list.push_back(data);
    db_response = this->db.insert_data(this->db_table, data_list);
    return { salt, session_hash };
}

int Sessions::remove(const std::string login) {
    try {
        this->db.delete_data_by("login", this->db_table, login);
    } catch (...) {
        return 1;
    }
    return 0;
}

int Sessions::remove(const std::string login, const std::string hash) {
    ResponseDataStruct db_response = this->db.get_data_by("login", this->db_table, login, { "hash" });
    if (db_response.status != StatusCode::ok) return -1;
    if (db_response.data[0].at("hash") != hash) return -2;

    try {
        this->db.delete_data_by("login", this->db_table, login);
    } catch (...) {
        return 1;
    }
    return 0;
}

CheckSessionStruct Sessions::check(std::string hash) {
    ResponseDataStruct sessions = this->db.get_data_by("hash", this->db_table, hash);
    if (sessions.status != StatusCode::ok)
        return { SessionCheckStatus::BAD_USER, "" };
    if (sessions.data[0]["hash"] != hash)
        return { SessionCheckStatus::BAD_PASSWORD, "" } ;
    return { SessionCheckStatus::OK, sessions.data[0]["login"] };
}

std::string Encode::salt(size_t length) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    std::ostringstream oss;
    oss << std::hex << std::setfill('0');

    for (size_t i = 0; i < length; ++i) {
        oss << std::setw(2) << dis(gen);
    }

    return oss.str();
}

std::string Encode::encode(const std::string& value, const std::string& salt) {
    const std::string data = salt + value;

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char*)data.c_str(), data.size(), hash);

    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }

    return ss.str();
}
