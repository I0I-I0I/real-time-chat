#include <cstdlib>
#include <unistd.h>
#include <winsock2.h>
#include "../logger/logger.h"
#include "./user.h"

User::User(SOCKET socket_) {
	this->socket = socket_;
	this->id = std::rand() % 1000;
}

void User::remove() {
	closesocket(this->socket);
	logger("Remove user: " + std::to_string(this->id) + '\n');
}

bool User::operator==(const User& other) const {
	return this->socket == other.socket;
}

int User::get_id() {
	return this->id;
}

SOCKET User::get_socket() {
	return this->socket;
}
