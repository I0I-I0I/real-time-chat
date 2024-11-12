#pragma once

#include <map>
#include <string>

using HttpHeadersStruct = std::map<std::string, std::string>;

struct HttpStruct {
	std::string method;
	std::string path;
	std::string version;
	HttpHeadersStruct headers;
	std::string body;
};

class Http {
public:
	static HttpStruct parce(std::string data);
	static std::string create(HttpStruct http_string);

private:
	static void log(HttpStruct http);
};
