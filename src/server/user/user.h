#pragma once

#include <functional>
#include <unistd.h>
#include <vector>
#include "../../logger/logger.h"

class User {
private:
	int user;

public:
	User(int socket);

	int get_user();
	void remove();
	bool operator==(const User& other) const;
};

