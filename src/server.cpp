#include <cstring>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "./socket/socket.h"

int main() {
	Socket server("localhost",  "64229");

	server.create("server");

	server.on("connection", [&server](int socket) -> void {
		server.send_msg(socket, "mHI from server\n");
	});

	server.on("chat", [&server](int socket) -> void {
		while (true) {
			server.receive_msg(socket);
			if (server.buffer[0] == 'q')
				break;
			else if (server.buffer[0] == 'm')
				server.send_msg(socket, "mOK\n");
			else if (server.buffer[0] == 'e')
				server.send_msg(socket, "mERROR\n");
			else
				server.send_msg(socket, "mWRONG MESSAGE TYPE\n");
		}
	});

	server.on("close", [&server](int socket) -> void {
		server.send_msg(socket, "mBYE\n");
	});

	server.launch();

	return 0;
}
