#include <string>
#include "../http/http.h"
#include "./handlers.h"

std::string HandlerOn::options(const HttpRequestStruct& http) {
	return Http::response(200, "", {
		{ "Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, PATCH, OPTIONS" },
		{ "Access-Control-Allow-Headers", "Content-Type, Authorization" },
		{ "Content-Length", "0" },
		{ "Connection", "keep-alive" }
	});
}
