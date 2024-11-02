#include <unistd.h>
#include "../../logger/logger.h"
#include "./user.h"

User::User(int socket) {
	this->user = socket;
}

void User::remove() {
	close(this->user);
	logger("Remove user: " + std::to_string(this->user) + '\n');
}

bool User::operator==(const User& other) const {
	return this->user == other.user;
}

int User::get_user() {
	return this->user;
}
