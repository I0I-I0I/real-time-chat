#include <string>
#include <cstring>
#include <sys/socket.h>
#include "./socket.h"

Socket::Socket(const char* host_, const char* port_, SocketOpts opts) {
	this->host = host_;
	this->port = port_;

	// TODO: It's not working now
	this->recv_timeout = opts.recv_timeout ? opts.recv_timeout : 0;
	this->send_timeout = opts.send_timeout ? opts.send_timeout : 0;

	this->backlog = opts.backlog ? opts.backlog : 5;
	this->log_level = opts.log_level != "" ? opts.log_level : "ERROR";
	this->users = {};
	for (auto& type_ : this->callback_types)
		this->callback_on[type_] = [](int, std::string) -> void {};
}

void Socket::start() {
	if (this->socket_type == "server") {
		this->start_listening();
		while (true)
			this->get_connection();
	} else if (this->socket_type == "client") {
		this->try_to_connect();
		socket_logger("Connection was established\n");
		this->connection_handler();
	}
}

void Socket::send_msg(int socket, std::string msg) {
	send(socket, msg.c_str(), msg.size(), 0);

	this->log_date(socket, "SEND", std::string(msg));
}

std::string Socket::receive_msg(int socket) {
	char* buffer_char = new char[BUFFER_SIZE];
	memset(buffer_char, 0, BUFFER_SIZE);
	recv(socket, buffer_char, BUFFER_SIZE, 0);

	this->buffer = std::string(buffer_char);

	this->log_date(socket, "RECV", this->buffer);
	return this->buffer;
}

int Socket::handle_received_data(int socket, std::string type, std::string msg) {
	if (type == "close" || msg == "") return CLOSE_CONNECTION;
	try {
		this->custom_callback_on[type](socket, msg);
	} catch (std::exception& e) {
		this->callback_on["*"](socket, msg);
	}
	return 0;
}

void Socket::on(std::string type, OnCallbackStruct callback) {
	if (type == "connection")
		this->create("server");
	else if (type == "open")
		this->create("client");

	for (int i = 0; i < this->callback_types.size(); i++) {
		if (this->callback_types[i] == type) {
			this->callback_on[type] = callback;
			return;
		}
	}
	this->custom_callback_on[type] = callback;
}

void Socket::send_all(int socket, std::string msg) {
	for (auto& user : this->users) {
		if (user.get_socket() == socket)
			this->send_msg(socket, msg);
		this->send_msg(user.get_socket(), (char *)msg.c_str());
	}
}

void Socket::shutdown_server() {
	this->close_users();
	this->close_socket();
}
