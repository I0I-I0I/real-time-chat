#include <functional>
#include <iostream>
#include <string>
#include "./config.h"
#include "./socket/socket.h"
#include "./handlers/handlers.h"
#include "./http/http.h"

std::string create_response(
		std::function<std::string(const HttpRequestStruct&)> handler,
		const HttpRequestStruct& info
) {
	TestOnHttpStruct test_result = Http::test_on_http(info);
	if (test_result.is_error)
		return test_result.response;
	return handler(info);
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
		if (info == "") return;
		HttpRequestStruct http = Http::parce(info);
		server.handle_received_data(socket, http.method, http);
	});

	server.on("GET", [&server](int socket, const auto& info) -> void {
		std::string response = create_response(HandlerOn::get, info);
		server.send_msg(socket, response);
	});

	server.on("POST", [&server](int socket, const auto& info) -> void {
		std::string response = create_response(HandlerOn::post, info);
		server.send_msg(socket, response);
	});

	server.on("OPTIONS", [&server](int socket, const auto& info) -> void {
		std::string response = create_response(HandlerOn::options, info);
		server.send_msg(socket, response);
	});

	server.on("DELETE", [&server](int socket, const auto& info) -> void {
		std::string response = create_response(HandlerOn::del, info);
		server.send_msg(socket, response);
	});

	server.on("PUT", [&server](int socket, const auto& info) -> void {
		std::string response = create_response(HandlerOn::put, info);
		server.send_msg(socket, response);
	});

	server.on("close", [&server](int socket, const auto& info) -> void {
		std::cout << "Connection closed" << std::endl;
	});

	server.start();

	return 0;
}
