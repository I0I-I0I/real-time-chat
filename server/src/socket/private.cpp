#include <string>
#include <thread>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
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
	this->socket_type = type_;
	this->addr = this->get_addr();
	this->main_socket = this->setup_socket();
	socket_logger("Socket was created");
}

int Socket::setup_socket() {
	struct addrinfo *p;
	int yes = 1;
	int sock;

	for (p = this->addr; p != NULL; p = p->ai_next) {
		if ((sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			error_handler(ERROR_SOCKET, "", false);
			continue;
		}
		if (this->socket_type == "client") break;
		if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(yes)) == -1)
			error_handler(ERROR_SETSOCKOPT, "", false);
		if (bind(sock, p->ai_addr, p->ai_addrlen) == -1) {
			close(sock);
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

	if (connect(this->main_socket, this->addr->ai_addr, this->addr->ai_addrlen) == -1) {
		error_handler(ERROR_CONNECT);
		close(this->main_socket);
	}

	inet_ntop(this->addr->ai_family, get_in_addr((struct sockaddr *)this->addr->ai_addr), s, sizeof s);
	socket_logger("client: connected to " + (std::string)s + ":" + this->port, "CONN");
}

void Socket::start_listening() {
	if (listen(this->main_socket, this->backlog) == -1)
		error_handler(ERROR_LISTEN);
	socket_logger("Server waiting on port " + std::string(this->port));
}

void Socket::connection_handler() {
	this->callback_on["open"](this->main_socket, this->buffer);
	this->callback_on["close"](this->main_socket, this->buffer);
	this->close_socket();
	socket_logger("Connection closed", "CONN");
}

void Socket::connection_handler(User user) {
	int user_socket = user.get_socket();
	this->callback_on["connection"](user_socket, this->buffer);
	this->callback_on["close"](user_socket, this->buffer);
	this->remove_user(user);
	socket_logger("Connection closed", "CONN");
}

int Socket::accept_connection() {
	char s[INET_ADDRSTRLEN];
	int socket;
	socklen_t sin_size;
	struct sockaddr_storage their_addr;

	sin_size = sizeof their_addr;
	if ((socket = accept(this->main_socket, (struct sockaddr *)&their_addr, &sin_size)) == -1)
		error_handler(ERROR_ACCEPT);

	inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
	socket_logger("server: got connection from " + (std::string)s + ":" + this->port, "CONN");
	return socket;
}

User Socket::wait_for_connection() {
	User user(this->accept_connection());
	this->users.push_back(user);
	socket_logger("User " + std::to_string(user.get_id()) + " connected", "CONN");
	return user;
}

void Socket::get_connection() {
	User user = this->wait_for_connection();
	std::thread ([this, user]() {
		User user_copy = user;
		this->connection_handler(user_copy);
		socket_logger("Count of users: " + std::to_string(this->users.size()));
	}).detach();
	socket_logger("Count of users: " + std::to_string(this->users.size()));
}

User Socket::get_current_user(int socket) {
	for (auto& user : this->users)
		if (user.get_socket() == socket)
			return user;
	socket_logger("User not found", "ERROR");
	return User(-1);
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
	close(this->main_socket);
}

void Socket::log_date(int socket, std::string log_type, std::string msg) {
	std::string log_msg = log_type;
	if (this->socket_type == "server") {
		User current_user = this->get_current_user(socket);
		log_msg += "(" + std::to_string(current_user.get_id()) + ")"
			": "+ msg;
	} else {
		log_msg += ": " + msg;
	}
	socket_logger(log_msg, "MSG");
}

void Socket::socket_logger(std::string msg, std::string type) {
	logger(msg, type);
}
