#include <string>
#include "../http/http.h"
#include "./handlers.h"

std::string HandlerOn::options(const HttpRequestStruct& http) {
	return Http::response(200, "", {
		{ "Content-Length", "0" }
	});
}