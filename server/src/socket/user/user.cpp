#include <cstdlib>
#include <unistd.h>
#include "../logger/logger.h"
#include "./user.h"

User::User(int socket_, std::string log_level_) {
	this->socket = socket_;
	this->id = std::rand() % 1000;
	this->log_level = log_level_ != "" ? log_level_ : "INFO";
}

void User::remove() {
	close(this->socket);
	this->socket_logger("User " + std::to_string(this->id) + " disconnected\n", "CONN");
}

bool User::operator==(const User& other) const {
	return this->socket == other.socket;
}

int User::get_id() {
	return this->id;
}

int User::get_socket() {
	return this->socket;
}

void User::socket_logger(std::string msg, std::string type) {
	logger(this->log_level, msg, type);
}