#include <iostream>
#include <string>
#include "./socket/socket.h"
#include "./handlers/handlers.h"
#include "./http/http.h"

int main() {
	SocketOpts opts = {
		.backlog = 5,
		.recv_timeout = 10000,
		.send_timeout = 10000,
	};
	Socket server("localhost",  "8080", opts);

	server.on("connection", [&server](int socket, std::string _) -> void {
		std::string info = server.receive_msg(socket);
		if (info == "") return;
		HttpRequestStruct http = Http::parce(info);
		server.handle_received_data(socket, http.method, http.body);
	});

	server.on("GET", [&server](int socket, std::string info) -> void {
		std::string response = HandlerOn::get("OK\n");
		server.send_msg(socket, response);
	});

	server.on("close", [&server](int socket, std::string info) -> void {
		std::cout << "Connection closed" << std::endl;
	});

	server.start();

	return 0;
}
