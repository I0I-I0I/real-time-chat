#include <iostream>
#include <string>
#include <thread>
#include "./socket/socket.h"
#include "./block/block.h"

// Timeout
// Opts
// Debug level

int main() {
	SocketOpts opts = {
		.log_level = "INFO"
	};
	Socket client("localhost", "64229", opts);

	std::thread t;
	bool flag = true;

	client.on("open", [&flag, &t, &client](SOCKET socket, std::string _) -> void {
		client.receive_msg(socket);
		client.send_msg(socket, "message", "Hi form client\n");

		t = std::thread([&flag, &client, socket]() -> void {
			while (true) {
				if (client.receive_msg(socket) == CLOSE_CONNECTION)
					break;
			}
			std::cout << "I'm was CLOSED\n";
			flag = false;
		});

		std::string msg;
		std::string type;
		while (flag) {
			std::getline(std::cin, msg);
			std::cout << "(type)> ";
			if (!flag) break;
			std::getline(std::cin, type);
			client.send_msg(socket, type, msg + '\n');

			/*Sleep(2000);*/
			/*client.send_msg(socket, "message", "How are u?\n");*/
		}
	});

	client.on("*", [&client](SOCKET socket, std::string info) -> void {
		Block::mouse(true);
		Sleep(1500);
		Block::mouse(false);
	});

	client.on("close", [&flag, &t, &client](SOCKET socket, std::string info) -> void {
		client.send_msg(socket, "close", "BYE\n");
		t.join();
	});

	client.start();

	return 0;
}
