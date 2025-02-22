#pragma once

#include <map>
#include <string>
#include <vector>
#include "json.hpp"

using json = nlohmann::json;

using HttpHeadersStruct = std::map<std::string, std::string>;

enum StatusCode {
    ok,
    bad_request,
    not_found,
    method_not_allowed,
    internal_server_error
};

struct StatucCodeArray {
    int code;
    std::string msg;
};

const std::unordered_map<StatusCode, StatucCodeArray> statusCodeMap = {
    {StatusCode::ok, {200, "OK"}},
    {StatusCode::bad_request, {400, "Bad Request"}},
    {StatusCode::not_found, {404, "Not Found"}},
    {StatusCode::method_not_allowed, {405, "Method Not Allowed"}},
    {StatusCode::internal_server_error, {500, "Internal Server Error"}}
};

/**
 * @param path (string)
 * @param params (map<string, string>)
 */
struct HttpPathStruct {
    std::vector<std::string> path;
    std::map<std::string, std::string> params;
};

/**
 * @param method (string)
 * @param url (HttpPathStruct)
 * @param version (string)
 * @param headers (HttpHeadersStruct)
 * @param body (string)
 */
struct HttpRequestStruct {
    std::string method;
    HttpPathStruct url;
    std::string version;
    HttpHeadersStruct headers;
    std::string body;
};

/**
 * @param status (string)
 * @param headers (HttpHeadersStruct)
 * @param body (string)
 */
struct HttpResponseStruct {
    std::string status;
    HttpHeadersStruct headers;
    std::string body;
};

/**
 * @param is_error (bool)
 * @param http (HttpRequestStruct)
 * @param response (string)
 */
struct HttpCastResultStruct  {
    bool is_error;
    HttpRequestStruct http;
    std::string response;
};

class Http {
public:
    /**
     * @brief Parse HTTP request
     * @param data (string)
     * @return (HttpRequestStruct)
     */
    static HttpRequestStruct parse(const std::string& request);

    /**
     * @brief Create HTTP response
     * @param code (int)
     * @param body (string)
     * @param headers = {} (map<string, string>)
     * @return (HttpResponseStruct)
     */
    static HttpResponseStruct response(StatusCode code, const std::string& body, HttpHeadersStruct headers = {});

    /**
     * @brief Transform HttpResponseStruct to string
     * @param http (HttpResponseStruct)
     * @return (string)
     */
    static std::string to_send(HttpResponseStruct& http);

    /**
     * @brief Transform HttpResponseStruct to vector<string>
     * @param http (HttpResponseStruct)
     * @param size (const int&)
     * @return (vector<string>)
     */
    static std::vector<std::string> to_send(HttpResponseStruct& http, int size);

private:
    static HttpPathStruct get_path(std::string path);

    static void log(HttpRequestStruct& http);
    static void log(HttpResponseStruct& http);
};
