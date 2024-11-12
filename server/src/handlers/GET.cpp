#include <string>
#include "../http/http.h"
#include "./handlers.h"

std::string HandlerOn::get(std::string data) {
	HttpResponseStruct response = {
		.status = "200 OK",
		.headers = {
			{ "Content-Type", "text/plain" },
			{ "Connection", "close" }
		},
		.body = data,
	};
	return Http::create(response);
}
