#include <iostream>
#include <string>
#include "./socket/socket.h"
#include "./handlers/handlers.h"
#include "./db/db.h"
#include "./config.h"
#include "./http/http.h"

int main() {
	SocketOpts opts = {
		.backlog = 5,
		.recv_timeout = 10000,
		.send_timeout = 10000,
	};
	Socket server("localhost",  "8080", opts);
	DB db(PATH_TO_DB);

	server.on("connection", [&server](int socket, auto _) -> void {
		std::string info = server.receive_msg(socket);
		if (info == "") return;
		HttpRequestStruct http = Http::parce(info);
		server.handle_received_data(socket, http.method, http);
	});

	server.on("GET", [&server](int socket, const auto& info) -> void {
		std::string response = HandlerOn::get(info);
		server.send_msg(socket, response);
	});

	server.on("POST", [&server](int socket, const auto& info) -> void {
		std::string response = HandlerOn::post(info);
		server.send_msg(socket, response);
	});

	server.on("close", [&server](int socket, const auto& info) -> void {
		std::cout << "Connection closed" << std::endl;
	});

	server.start();

	return 0;
}
