#pragma once

#include <map>
#include <string>

using HttpHeadersStruct = std::map<std::string, std::string>;

/**
 * @param path (string)
 * @param type (string)
 * @param params (map<string, string>)
 */
struct HttpPathStruct {
	std::string path;
	std::string type;
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
	HttpPathStruct path;
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

class Http {
public:
	/**
	 * @brief Parce HTTP request
	 * @param data (string)
	 * @return HttpRequestStruct
	 */
	static HttpRequestStruct parce(const std::string& request);

	/**
	 * @brief Preparing HTTP response for sending
	 * @param http (HttpResponseStruct)
	 * @return string
	 */
	static std::string to_send(HttpResponseStruct http);

private:
	static void http_log(HttpRequestStruct& http);
	static HttpPathStruct get_path(std::string path);
};
