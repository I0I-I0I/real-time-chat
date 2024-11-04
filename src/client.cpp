#include <iostream>
#include <string>
#include <thread>
#include "./socket/socket.h"
#include "./block/block.h"

int main() {
	Socket client("localhost", "64229");

	std::thread t;

	client.on("open", [&t, &client](SOCKET socket, std::string _) -> void {
		client.receive_msg(socket);
		client.send_msg(socket, "message", "Hi form client\n");

		std::string msg;
		std::string type;
		while (true) {
			Sleep(2000);
			client.send_msg(socket, "message", "How are u?\n");
			if (client.receive_msg(socket) == CLOSE_CONNECTION)
				break;
		}
	});

	client.on("*", [&client](SOCKET socket, std::string info) -> void {
		Block::mouse(true);
		Sleep(1500);
		Block::mouse(false);
	});

	client.on("close", [&t, &client](SOCKET socket, std::string info) -> void {
		client.send_msg(socket, "close", "BYE\n");
	});

	client.start();

	return 0;
}
