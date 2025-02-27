#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include "json.hpp"
#include "../logger/logger.h"
#include "../config.h"
#include "./http.h"

using json = nlohmann::json;

HttpRequestStruct Http::parse(const std::string& request) {
    HttpRequestStruct http;

    std::regex request_line_pattern(R"((\w+) ([^\s]+) HTTP/(\d.\d))");
    std::regex header_pattern(R"(([^:]+):\s?(.+))");
    std::smatch matches;

    std::istringstream stream(request);
    std::string line;

    std::getline(stream, line);
    if (std::regex_search(line, matches, request_line_pattern)) {
        if (matches.size() == 4) {
            http.method = matches[1].str();
            http.url = get_path(matches[2].str());
            http.version = matches[3].str();
        }
    }

    HttpHeadersStruct headers;
    while (std::getline(stream, line) && (line != "\r")) {
        if (std::regex_search(line, matches, header_pattern)) {
            if (matches.size() == 3) {
                std::string key = matches[1].str();
                std::string value = matches[2].str();
                std::transform(key.begin(), key.end(), key.begin(),
                               [](unsigned char c){ return std::tolower(c); });
                std::transform(value.begin(), value.end(), value.begin(),
                               [](unsigned char c){ return std::tolower(c); });
                headers[key] = value;
            }
        }
    }
    http.headers = headers;

    const auto content_length_iter = headers.find("content-length");
    if (content_length_iter == headers.end()) {
        http.body = "";
        Http::log(http);
        return http;
    }

    int length;
    try {
        length = std::stoi(content_length_iter->second);
    } catch (const std::invalid_argument&) {
        logger("Invalid Content-Length value", "ERROR");
    }

    std::string body(length, '\0');
    if (stream.read(&body[0], length)) {
        http.body = body;
    } else {
        logger("Error reading from stream while reading body", "ERROR");
    }

    return http;
}

HttpResponseStruct Http::response(StatusCode code, const std::string& body, HttpHeadersStruct headers) {
    HttpResponseStruct http;
    http.status = std::to_string(StatusCodeMap.at(code).code) + " " + StatusCodeMap.at(code).msg;

    if (headers.find("content-type") == headers.end()) headers["content-type"] = "plain/text";
    if (headers.find("connection") == headers.end()) headers["connection"] = "close";
    headers["content-length"] = std::to_string(body.size());

    http.headers = headers;
    http.body = body;

    return http;
}

std::string Http::to_send(HttpResponseStruct& http) {
    std::ostringstream response;

    response << "HTTP/1.1 " + http.status + " \r\n";

    http.headers["access-control-allow-origin"] = "*";
    for (const auto& header : http.headers)
        response << header.first + ": " + header.second + "\r\n";
    response << "\r\n";
    response << http.body;

    Http::log(http);

    return response.str();
}

std::vector<std::string> Http::to_send(HttpResponseStruct& http, int size) {
    std::vector<std::string> response;
    std::ostringstream top;

    top << "HTTP/1.1 " + http.status + " \r\n";

    http.headers["access-control-allow-origin"] = "*";
    http.headers["transfer-encoding"] = "chunked";
    http.headers.erase("content-length");
    for (const auto& header : http.headers)
        top << header.first + ": " + header.second + "\r\n";
    top << "\r\n";

    response.push_back(top.str());

    unsigned int cursor = 0;
    while (cursor < http.body.size()) {
        int chunk_size = std::min(size, static_cast<int>(http.body.size() - cursor));
        std::ostringstream chunk;
        chunk << std::hex << chunk_size << "\r\n" << http.body.substr(cursor, chunk_size) << "\r\n";
        response.push_back(chunk.str());
        cursor += chunk_size;
    }
    response.push_back("0\r\n\r\n");

    Http::log(http);

    return response;
}
