#include <functional>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <algorithm>
#include "./logger/logger.h"
#include "./user/user.h"
#include "./socket.h"

#pragma comment(lib, "Ws2_32.lib")

void* Socket::get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET)
		return &(((struct sockaddr_in*)sa)->sin_addr);
	return  &(((struct sockaddr_in6*)sa)->sin6_addr);
}

struct addrinfo* Socket::get_addr() {
	struct addrinfo hints, *servinfo, *p;
	int rv;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((rv = getaddrinfo(this->host, this->port, &hints, &servinfo)) != 0)
		error_handler(ERROR_GET_ADDR, gai_strerror(rv), false);

	return servinfo;
}

void Socket::create(std::string type_) {
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		error_handler(ERROR_WSA_STARTUP);

	this->socket_type = type_;
	this->addr = this->get_addr();
	this->main_socket = this->setup_socket();
	logger("Socket was created\n");
}

SOCKET Socket::setup_socket() {
	struct addrinfo *p;
	int yes = 1;
	SOCKET sock;

	for (p = this->addr; p != NULL; p = p->ai_next) {
		if ((sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == INVALID_SOCKET) {
			error_handler(ERROR_SOCKET, "", false);
			continue;
		}
		if (this->socket_type == "client") break;
		if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(yes)) == SOCKET_ERROR)
			error_handler(WSAGetLastError());
		if (bind(sock, p->ai_addr, p->ai_addrlen) == -1) {
			closesocket(sock);
			error_handler(ERROR_BIND, "", false);
			continue;
		}
		break;
	}

	if (p == NULL)
		error_handler(ERROR_BIND);

	return sock;
}

void Socket::try_to_connect() {
	char s[INET6_ADDRSTRLEN];

	if (connect(this->main_socket, this->addr->ai_addr, this->addr->ai_addrlen) == SOCKET_ERROR) {
		std::cout << "Connection error" << WSAGetLastError() << std::endl;
		error_handler(ERROR_CONNECT);
		closesocket(this->main_socket);
	}

	inet_ntop(this->addr->ai_family, get_in_addr((struct sockaddr *)this->addr->ai_addr), s, sizeof s);
	logger("client: connected to " + (std::string)s + ":" + this->port + '\n');
}

void Socket::start_listening() {
	if (listen(this->main_socket, this->backlog) == SOCKET_ERROR)
		error_handler(ERROR_LISTEN);
	logger("Server waiting on port " + std::string(this->port) + " \n");
}

void Socket::connection_handler() {
	this->callback_on["open"](this->main_socket, this->_buffer.msg);
	this->callback_on["close"](this->main_socket, this->_buffer.msg);
	this->close_socket();
	logger("Connection closed\n");
}

void Socket::connection_handler(User user) {
	SOCKET user_socket = user.get_socket();
	this->callback_on["connection"](user_socket, this->_buffer.msg);
	this->callback_on["close"](user_socket, this->_buffer.msg);
	this->remove_user(user);
	logger("Connection closed\n");
}

SOCKET Socket::accept_connection() {
	char s[INET_ADDRSTRLEN];
	SOCKET socket;
	socklen_t sin_size;
	struct sockaddr_storage their_addr;

	sin_size = sizeof their_addr;
	if ((socket = accept(this->main_socket, (struct sockaddr *)&their_addr, &sin_size)) == INVALID_SOCKET) {
		std::cout << "Accept error: " << WSAGetLastError() << std::endl;
		error_handler(ERROR_ACCEPT);
	}

	inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
	logger("server: got connection from " + (std::string)s + ":" + this->port + '\n');
	return socket;
}

User Socket::wait_for_connection() {
	User user(this->accept_connection());
	this->users.push_back(user);
	logger("User " + std::to_string(user.get_id()) + " connected" + '\n');
	return user;
}

void Socket::get_connection() {
	User user = this->wait_for_connection();
    std::thread ([this, user]() {
		User user_copy = user;
		this->connection_handler(user_copy);
		logger("Count of users: " + std::to_string(this->users.size()) + '\n');
	}).detach();
	logger("Count of users: " + std::to_string(this->users.size()) + '\n');
}

User Socket::get_current_user(SOCKET socket) {
	for (auto& user : this->users)
		if (user.get_socket() == socket)
			return user;
	logger("User not found\n", "ERROR");
	return User();
}

void Socket::remove_user(User& user) {
	user.remove();
	this->users.erase(
		std::remove(this->users.begin(), this->users.end(), user),
		this->users.end()
	);
}

void Socket::close_users() {
	for (auto& user : this->users)
		user.remove();
}

void Socket::close_socket() {
	closesocket(this->main_socket);
	WSACleanup();
}

void Socket::log_date(SOCKET socket, std::string log_type, std::string msg) {
	std::string log_msg = log_type;
	if (this->socket_type == "server") {
		User current_user = this->get_current_user(socket);
		log_msg += "(" + std::to_string(current_user.get_id()) + ")"
			": "+ msg;
	} else {
		log_msg += ": " + msg;
	}
	logger(log_msg, log_type);
}

