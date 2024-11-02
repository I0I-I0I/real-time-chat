#include <functional>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "../consts.h"
#include "../logger/logger.h"
#include "./socket.h"

Socket::Socket(const char* host_, const char* port_, int backlog_) {
	this->host = host_;
	this->port = port_;
	this->backlog = backlog_;
	this->users = {};
	this->callback_types = { "connection", "open", "chat", "close" };
	for (auto& type_ : this->callback_types)
		this->callback_on[type_] = [](int) -> void {};
}

void Socket::create(std::string type_) {
	this->socket_type = type_;
	this->addr = this->get_addr();
	this->main_socket = this->setup_socket(this->socket_type);
	logger("Socket was created\n");
}

void Socket::launch() {
	this->start_listening();
	logger("Waiting for connection...\n");
	while (true)
		this->get_connection();
}

void Socket::connection() {
	this->try_connect();
	logger("Connection was established\n");

	this->callback_on["open"](this->main_socket);
	this->callback_on["chat"](this->main_socket);
	this->callback_on["close"](this->main_socket);

	this->close_socket();
	logger("Client was closed\n");
}

void Socket::send_msg(int socket, std::string msg) {
	send(socket, msg.c_str(), msg.size(), 0);

	std::string log_msg;
	if (this->socket_type == "server") {
		User current_user = this->get_current_user(socket);
		log_msg = "Send"
			"(" + std::to_string(current_user.get_id()) + ")"
			": " + msg;
	} else {
		log_msg = "Send: " + msg;
	}
	logger(log_msg, "SEND");
}

void Socket::receive_msg(int socket) {
	memset(this->buffer, 0, BUFFER_SIZE);
	recv(socket, this->buffer, BUFFER_SIZE, 0);

	std::string log_msg;
	if (this->socket_type == "server") {
		User current_user = this->get_current_user(socket);
		log_msg = "Received"
			"(" + std::to_string(current_user.get_id()) + ")"
			": "+ std::string(this->buffer);
	} else {
		log_msg = "Received: " + std::string(this->buffer);
	}
	logger(log_msg, "RECV");
}

void Socket::ping(int socket, std::string msg) {
	this->send_msg(socket, msg);
	this->receive_msg(socket);
}

void Socket::on(std::string type, std::function<void(int socket)> callback) {
	for (auto& type_ : this->callback_types) {
		if (type_ == type) {
			this->callback_on[type] = callback;
			return;
		}
	}
	error_handler(ERROR_WRONG_TYPE_FOR_ON);
}

void Socket::shutdown_server() {
	this->close_users();
	this->close_socket();
}
