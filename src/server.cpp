#include <string>
#include "./socket/socket.h"

int main() {
	SocketOpts opts = {
		.backlog = 5,
		.recv_timeout = 10000,
		.send_timeout = 10000,
		.log_level = "INFO"
	};
	Socket server("localhost",  "64229", opts);

	server.on("connection", [&server](int socket, std::string info) -> void {
		server.send_msg(socket, "message", "HI from server\n");
		while (true)
			if (server.receive_msg(socket) == CLOSE_CONNECTION)
				break;
	});

	server.on("message", [&server](int socket, std::string info) -> void {
		server.send_msg(socket, "message", "OK\n");
	});

	server.on("error", [&server](int socket, std::string info) -> void {
		server.send_msg(socket, "message", "OK\n");
	});

	server.on("all", [&server](int socket, std::string info) -> void {
		server.send_all(socket, "message", info);
	});

	server.on("*", [&server](int socket, std::string info) -> void {
		server.send_msg(socket, "error", "Wrong message type\n");
	});

	server.on("close", [&server](int socket, std::string info) -> void {
		server.send_msg(socket, "close", "BYE\n");
	});

	server.start();

	return 0;
}
