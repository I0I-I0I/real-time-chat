#include <any>
#include <string>
#include "../http/http.h"
#include "./handlers.h"

std::string HandlerOn::get(const std::any& data) {
	if (data.type() != typeid(HttpRequestStruct)) return Http::to_send({
		.status = "400 Bad Request",
		.headers = {
			{ "Content-Type", "text/plain" },
			{ "Connection", "close" }
		},
		.body = "",
	});

	auto http = std::any_cast<HttpRequestStruct>(data);
	HttpResponseStruct response = {
		.status = "200 OK",
		.headers = {
			{ "Content-Type", "text/plain" },
			{ "Connection", "close" }
		},
		.body = http.body,
	};
	return Http::to_send(response);
}
