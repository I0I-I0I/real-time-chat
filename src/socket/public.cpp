#include <string>
#include <cstring>
#include <sys/socket.h>
#include "./logger/logger.h"
#include "./packet/packet.h"
#include "./socket.h"

Socket::Socket(const char* host_, const char* port_, int backlog_) {
	this->host = host_;
	this->port = port_;
	this->backlog = backlog_;
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
		logger("Connection was established\n");
		this->connection_handler_client();
	}
}

void Socket::send_msg(int socket, std::string type, std::string msg) {
	PacketStruct packet;
	packet.type = type;
	packet.msg = msg;

	PacketStrStruct packet_str = Packet::create(packet);

	send(socket, packet_str.msg, packet_str.length, 0);

	this->log_date(socket, "SEND", packet.type + ":" + packet.msg);
}

int Socket::receive_msg(int socket) {
	char* buffer_char = new char[BUFFER_SIZE];
	memset(buffer_char, 0, BUFFER_SIZE);
	recv(socket, buffer_char, BUFFER_SIZE, 0);

	PacketStruct packet = Packet::parce(buffer_char);
	delete[] buffer_char;

	this->_buffer.msg = packet.msg;
	this->_buffer.type = packet.type;

	this->log_date(socket, "RECV", packet.type + ":" + packet.msg);

	if (packet.type == "close") return CLOSE_CONNECTION;
	try {
		this->custom_callback_on[this->_buffer.type](socket, this->_buffer.msg);
	} catch (std::exception& e) {
		this->callback_on["*"](socket, this->_buffer.msg);
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

void Socket::send_all(int socket, std::string type, std::string msg) {
	for (User user : this->users) {
		if (user.get_socket() == socket)
			continue;
		this->send_msg(user.get_socket(), type, msg);
	}
}

void Socket::shutdown_server() {
	this->close_users();
	this->close_socket();
}
