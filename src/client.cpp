#include <iostream>
#include <string>
#include "./socket/socket.h"

int main() {
	Socket client("localhost", "64229");

	client.create("client");

	client.on("open", [&client](int socket) -> void {
		client.receive_msg(socket);
		client.send_msg(socket, "mHi form client\n");
	});

	client.on("chat", [&client](int socket) -> void {
		std::string msg;
		while (true) {
			client.receive_msg(socket);
			std::cout << "Prompt> ";
			std::getline(std::cin, msg);
			if (msg == "q") break;
			client.send_msg(socket, "m" + msg + "\n");
		}
	});

	client.on("close", [&client](int socket) -> void {
		client.send_msg(socket, "q\n");
		client.receive_msg(socket);
	});

	client.connection();

	return 0;
}
