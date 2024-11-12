#include <iostream>
#include <string>
#include "./socket/socket.h"
#include "./http/http.h"

int main() {
	SocketOpts opts = {
		.backlog = 5,
		.recv_timeout = 10000,
		.send_timeout = 10000,
	};
	Socket server("localhost",  "8080", opts);

	server.on("connection", [&server](int socket, std::string _) -> void {
		while (true) {
			std::string info = server.receive_msg(socket);
			if (info == "") break;
			HttpStruct http = Http::parce(info);

			if (server.handle_received_data(socket, http.method, http.body) == CLOSE_CONNECTION)
				break;
		}
	});

	server.on("GET", [&server](int socket, std::string info) -> void {
		std::cout << "Method: GET" << std::endl;
	});

	server.start();

	return 0;
}
