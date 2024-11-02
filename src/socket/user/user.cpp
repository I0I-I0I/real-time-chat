#include <cstdlib>
#include <unistd.h>
#include "../../logger/logger.h"
#include "./user.h"

User::User(int socket_) {
	this->user = socket_;
	this->id = std::rand() % 1000;
}

void User::remove() {
	close(this->user);
	logger("Remove user: " + std::to_string(this->id) + '\n');
}

bool User::operator==(const User& other) const {
	return this->user == other.user;
}

int User::get_id() {
	return this->id;
}

int User::get_user() {
	return this->user;
}
