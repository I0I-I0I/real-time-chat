#pragma once

#include <map>
#include <string>

using HttpHeadersStruct = std::map<std::string, std::string>;

struct HttpPathStruct {
	std::string path;
	std::string type;
	std::map<std::string, std::string> params;
};

struct HttpRequestStruct {
	std::string method;
	HttpPathStruct path;
	std::string version;
	HttpHeadersStruct headers;
	std::string body;
};

struct HttpResponseStruct {
	std::string status;
	HttpHeadersStruct headers;
	std::string body;
};

class Http {
public:
	static HttpRequestStruct parce(std::string data);
	static std::string to_send(HttpResponseStruct http);

private:
	static void log(HttpRequestStruct http);
	static HttpPathStruct get_path(std::string path);
};
