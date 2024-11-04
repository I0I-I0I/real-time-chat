#include <iostream>
#include <winsock2.h>
#include <string>
#include <thread>
#include "./socket/socket.h"

int main() {
	Socket client("localhost", "64229");

	std::thread t;

	client.on("open", [&t, &client](SOCKET socket, std::string _) -> void {
		client.receive_msg(socket);
		client.send_msg(socket, "message", "Hi form client\n");

		t = std::thread([&client, socket]() -> void {
			while (true) {
				if (client.receive_msg(socket) == CLOSE_CONNECTION)
					break;
			}
			std::cout << "I'm was CLOSED\n";
		});

		std::string msg;
		std::string type;
		while (true) {
			std::getline(std::cin, msg);
			if (msg == "q") break;
			std::cout << "(type)> ";
			std::getline(std::cin, type);
			client.send_msg(socket, type, msg + '\n');
		}
	});

	client.on("*", [&client](SOCKET socket, std::string info) -> void {
	});

	client.on("close", [&t, &client](SOCKET socket, std::string info) -> void {
		client.send_msg(socket, "close", "BYE\n");
		t.join();
	});

	client.start();

	return 0;
}
