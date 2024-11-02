#include <string>
#include <thread>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "../logger/logger.h"
#include "../consts.h"
#include "./user/user.h"
#include "./server.h"

void* Server::get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET)
		return &(((struct sockaddr_in*)sa)->sin_addr);
	return  &(((struct sockaddr_in6*)sa)->sin6_addr);
}

struct addrinfo* Server::get_addr() {
	struct addrinfo hints, *servinfo, *p;
	int rv;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((rv = getaddrinfo(NULL, this->port, &hints, &servinfo)) != 0) {
		std::string str = gai_strerror(rv);
		error_handler(ERROR_GET_ADDR, str, false);
	}

	return servinfo;
}

int Server::setup_socket() {
	struct addrinfo *p;
	int yes = 1;
	int sock;

	for (p = this->addr; p != NULL; p = p->ai_next) {
		if ((sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			error_handler(ERROR_SOCKET, "", false);
			continue;
		}
		if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
			error_handler(ERROR_SETSOCKOPT);
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

void Server::start_listening() {
	if (listen(server_socket, this->backlog) == -1)
		error_handler(ERROR_LISTEN);
}

int Server::accept_connection() {
	char s[INET_ADDRSTRLEN];
	int socket;
	socklen_t sin_size;
	struct sockaddr_storage their_addr;

	sin_size = sizeof their_addr;
	socket = accept(this->server_socket, (struct sockaddr *)&their_addr, &sin_size);
	if (socket == -1)
		error_handler(ERROR_ACCEPT);

	inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
	logger("server: got connection from " + (std::string)s + '\n');
	return socket;
}

void Server::chating(int socket) {
	while (true) {
		this->receive_msg(socket);
		if (this->buffer[0] == 'q')
			break;
		else if (this->buffer[0] == 'm')
			this->callback_on["message"](socket, this->buffer);
		else if (this->buffer[0] == 'e')
			this->callback_on["error"](socket, this->buffer);
		else
			this->send_msg(socket, "WRONG MESSAGE TYPE\n");
	}
}

void Server::connection_handler(User user) {
	int user_socket = user.get_user();
	this->callback_on["connection"](user_socket, (char*)"");
	this->chating(user_socket);
	this->callback_on["close"](user_socket, (char*)"");
	user.remove();
	logger("Connection closed\n");
}

User Server::wait_for_connection() {
	User user(this->accept_connection());
	this->users.push_back(user);
	return user;
}

void Server::remove_user(User user) {
	this->users.erase(
		std::remove(this->users.begin(), this->users.end(), user),
		this->users.end()
	);
}

void Server::get_connection() {
	User user = this->wait_for_connection();
	std::thread (([this, user]() -> void {
		this->connection_handler(user);
		this->remove_user(user);
		logger("Count of users (after remove): " + std::to_string(this->users.size()) + '\n');
	})).detach();
	logger("Count of users: " + std::to_string(this->users.size()) + '\n');
}

void Server::close_users() {
	for (auto& user : this->users)
		user.remove();
}

void Server::close_server() {
	close(this->server_socket);
}
