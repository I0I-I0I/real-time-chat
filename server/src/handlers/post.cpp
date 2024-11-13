#include <any>
#include <string>
#include "../http/http.h"
#include "./handlers.h"

std::string HandlerOn::post(const std::any& data) {
	HttpResponseStruct response = {
		.status = "200 OK",
		.headers = {
			{ "Content-Type", "text/plain" },
			{ "Connection", "close" }
		},
	};
	return Http::to_send(response);
}
