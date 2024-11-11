#include <iostream>
#include <string>
#include <thread>
#include "./socket/socket.h"

int main() {
	SocketOpts opts = {
		.log_level = "INFO"
	};
	Socket client("localhost", "64230", opts);

	std::thread t;
	bool flag = true;

	client.on("open", [&flag, &t, &client](int socket, std::string _) -> void {
		client.receive_msg(socket);
		client.send_msg(socket, "message", "Hi form client\n");

		t = std::thread([&flag, &client, socket]() -> void {
			while (true)
				if (client.receive_msg(socket) == CLOSE_CONNECTION) break;
			flag = false;
		});

		std::string msg;
		std::string type;
		while (flag) {
			std::getline(std::cin, msg);
			if (!flag || msg == "q") break;
			std::cout << "(type)> ";
			std::getline(std::cin, type);
			client.send_msg(socket, type, msg + '\n');
		}
	});

	client.on("*", [&client](int socket, std::string info) -> void {
	});

	client.on("close", [&flag, &t, &client](int socket, std::string info) -> void {
		client.send_msg(socket, "close", "BYE\n");
		t.join();
	});

	client.start();

	return 0;
}
