#include <string>
#include <winsock2.h>
#include "./socket/socket.h"

int main() {
	SocketOpts opts = {
		.backlog = 5,
		.timeout = 10000,
	};
	Socket server("localhost",  "64229", opts);

	server.on("connection", [&server](SOCKET socket, std::string info) -> void {
		server.send_msg(socket, "message", "HI from server\n");
		while (true) {
			if (server.receive_msg(socket) == CLOSE_CONNECTION)
				break;
		}
	});

	server.on("message", [&server](SOCKET socket, std::string info) -> void {
		server.send_msg(socket, "message", "OK\n");
	});

	server.on("error", [&server](SOCKET socket, std::string info) -> void {
		server.send_msg(socket, "message", "OK\n");
	});

	server.on("all", [&server](SOCKET socket, std::string info) -> void {
		server.send_all(socket, "message", info);
	});

	server.on("*", [&server](SOCKET socket, std::string info) -> void {
		server.send_msg(socket, "error", "Wrong message type\n");
	});

	server.on("close", [&server](SOCKET socket, std::string info) -> void {
		server.send_msg(socket, "close", "BYE\n");
	});

	server.start();

	return 0;
}
