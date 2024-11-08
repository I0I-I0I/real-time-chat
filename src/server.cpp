#include <string>
#include "./socket/socket.h"
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
			std::string str_packet = Packet::create({
			.type = "message",
			.msg = "OK\n"
		});
		server.send_msg(socket, str_packet);
	});

	server.on("error", [&server](int socket, std::string info) -> void {
		std::string str_packet = Packet::create({
			.type = "message",
			.msg = "OK\n"
		});
		server.send_msg(socket, str_packet);
	});

	server.on("all", [&server](int socket, std::string info) -> void {
		std::string str_packet = Packet::create({
			.type = "message",
			.msg = "OK\n"
		});
		server.send_all(socket, str_packet);
	});

	server.on("*", [&server](int socket, std::string info) -> void {
		std::string str_packet = Packet::create({
			.type = "error",
			.msg = "Wrong message type\n"
		});
		server.send_msg(socket, str_packet);
	});

	server.on("close", [&server](int socket, std::string info) -> void {
		std::string str_packet = Packet::create({
			.type = "close",
			.msg = "BYE\n"
		});
		server.send_msg(socket, str_packet);
	});

	server.start();

	return 0;
}
