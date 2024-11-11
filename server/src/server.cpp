#include <string>
#include "./socket/socket.h"
#include "./handlers/handlers.h"
#include "./packet/packet.h"

int main() {
	SocketOpts opts = {
		.backlog = 5,
		.recv_timeout = 10000,
		.send_timeout = 10000,
		.log_level = "INFO"
	};
	Socket server("localhost",  "8080", opts);

	server.on("connection", [&server](int socket, std::string _) -> void {
		while (true) {
			std::string info = server.receive_msg(socket);
			PacketStruct packet = Packet::parce((char*)info.c_str());
			if (server.handle_received_data(socket, packet.type, packet.msg) == CLOSE_CONNECTION)
				break;
		}
	});

	server.on("message", [&server](int socket, std::string info) -> void {
		std::string response = HandlerOn::message(info);
		server.send_msg(socket, response);
	});

	server.on("error", [&server](int socket, std::string info) -> void {
		std::string response = HandlerOn::error(info);
		server.send_msg(socket, response);
	});

	server.on("data", [&server](int socket, std::string info) -> void {
		std::string response = HandlerOn::data(info);
		server.send_msg(socket, response);
	});

	server.on("all", [&server](int socket, std::string info) -> void {
		std::string response = HandlerOn::all(info);
		server.send_msg(socket, response);
	});

	server.on("*", [&server](int socket, std::string info) -> void {
		std::string response = HandlerOn::other(info);
		server.send_msg(socket, response);
	});

	server.on("close", [&server](int socket, std::string info) -> void {
		std::string response = HandlerOn::close(info);
		server.send_msg(socket, response);
	});

	server.start();

	return 0;
}
