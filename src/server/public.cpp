#include <functional>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "../consts.h"
#include "../logger/logger.h"
#include "./server.h"

Server::Server(const char* port_, int backlog_) {
	this->port = port_;
	this->backlog = backlog_;
	this->users = {};
	this->callback_types = { "connection", "message", "error", "close" };
	for (auto& type_ : this->callback_types)
		this->callback_on[type_] = [](int, char*) -> void {};
}

void Server::create() {
	this->addr = this->get_addr();
	this->server_socket = this->setup_socket();
	logger("Server was created\n");
}

void Server::launch() {
	this->start_listening();
	logger("Waiting for connection...\n");
	while (true)
		this->get_connection();
}

void Server::send_msg(int socket, std::string msg) {
	send(socket, msg.c_str(), msg.size(), 0);
	logger("Send: " + msg, "SEND");
}

void Server::receive_msg(int socket) {
	memset(this->buffer, 0, BUFFER_SIZE);
	recv(socket, this->buffer, BUFFER_SIZE, 0);
	logger("Received: " + std::string(this->buffer), "RECV");
}

void Server::on(std::string type, std::function<void(int socket, char*)> callback) {
	for (auto& type_ : this->callback_types) {
		if (type_ == type) {
			this->callback_on[type] = callback;
			return;
		}
	}
	error_handler(ERROR_WRONG_TYPE_FOR_ON);
}

void Server::shutdown_server() {
	this->close_users();
	this->close_server();
}
