#pragma once

#include <any>
#include <map>
#include <string>
#include <vector>
#include "../../lib/json.hpp"

using json = nlohmann::json;

using HttpHeadersStruct = std::map<std::string, std::string>;

/**
 * @param path (string)
 * @param type (string)
 * @param params (map<string, string>)
 */
struct HttpPathStruct {
	std::vector<std::string> path;
	std::map<std::string, std::string> params;
};

/**
 * @param method (string)
 * @param path (HttpPathStruct)
 * @param version (string)
 * @param headers (map<string, string>)
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
 * @param headers (map<string, string>)
 * @param body (string)
 */
struct HttpResponseStruct {
	std::string status;
	HttpHeadersStruct headers;
	std::string body;
};

struct HttpCastResultStruct  {
	bool is_error;
	HttpRequestStruct http;
	std::string response;
};


class Http {
public:
	/**
	 * @brief Parce HTTP request
	 * @param data (string)
	 * @return HttpRequestStruct
	 */
	static HttpRequestStruct parce(const std::string& request);

	/**
	 * @brief Create HTTP response
	 * @param code (int)
	 * param body (string)
	 * @param headers = {} (map<string, string>)
	 * @return string
	 */
	static std::string response(int code, std::string body, HttpHeadersStruct headers = {});

	/**
	 * @brief Test on HTTP request
	 * @param data (const any&)
	 * @return TestOnHttpStruct
	 */
	static HttpCastResultStruct validate(const std::any& data);

private:
	static std::string to_send(HttpResponseStruct http);
	static HttpPathStruct get_path(std::string path);
	static std::string get_status(int code);

	static void log(HttpRequestStruct http);
	static void log(HttpResponseStruct http);
};
