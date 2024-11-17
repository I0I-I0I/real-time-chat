#include <functional>
#include <iostream>
#include <string>
#include "../lib/json.hpp"
#include "./config.h"
#include "./socket/socket.h"
#include "./handlers/handlers.h"
#include "./http/http.h"

using json = nlohmann::json;

std::string create_response(
		std::function<std::string(const HttpRequestStruct&)> handler,
		const std::any& info
) {
	HttpCastResultStruct cast_result = Http::cast(info);
	if (cast_result.is_error)
		return cast_result.response;
	return handler(cast_result.http);
}

int main() {
	SocketOpts opts = {
		.backlog = 5,
		.recv_timeout = 10000,
		.send_timeout = 10000,
	};
	Socket server("localhost",  "8080", opts);

	server.on("connection", [&server](int socket, const auto& _) -> void {
		std::string info = server.receive_msg(socket);
		if (info == "")
			return;
		HttpRequestStruct http = Http::parce(info);
		server.handle_received_data(socket, http.method, http);
	});

	server.on("close", [&server](int socket, const auto& info) -> void {
		std::cout << "Connection closed" << std::endl;
	});

	server.on("*", [&server](int socket, const auto& info) -> void {
		json response = {
			{ "status", "Method not allowed" },
			{ "msg", "Unknown method" }
		};
		server.send_msg(socket, Http::response(405, response.dump(), {
			{ "Content-Type", "application/json" }
		}));
	});

	std::map<std::string, HandlerOnFunc> method_handlers = {
		{ "GET", HandlerOn::get },
		{ "POST", HandlerOn::post },
		{ "OPTIONS", HandlerOn::options },
		{ "DELETE", HandlerOn::del },
		{ "PUT", HandlerOn::put }
	};

	for (const auto& [method, handler] : method_handlers)
		server.on(method, [&server, &handler](int socket, const auto& info) -> void {
			std::string response = create_response(handler, info);
			server.send_msg(socket, response);
		});

	server.start();

	return 0;
}
