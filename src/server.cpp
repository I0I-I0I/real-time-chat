#include <cstring>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "./server/server.h"

#define PORT "64229"

int main() {
	Server server(PORT);

	server.create();

	server.on("connection", [&server](int socket, char* buffer) -> void {
		server.send_msg(socket, "HI\n");
	});
	server.on("message", [&server](int socket, char* buffer) -> void {
		server.send_msg(socket, "OK\n");
	});
	server.on("error", [&server](int socket, char* buffer) -> void {
		server.send_msg(socket, "ERROR\n");
	});
	server.on("close", [&server](int socket, char* buffer) -> void {
		server.send_msg(socket, "BYE\n");
	});

	server.launch();

	return 0;
}
