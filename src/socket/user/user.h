#pragma once

#include <functional>
#include <unistd.h>
#include <vector>
#include "../../logger/logger.h"

class User {
private:
	int user;
	int id;

public:
	User(int socket_ = 0);

	int get_user();
	int get_id();
	void set_id(int id_);
	void remove();
	bool operator==(const User& other) const;
};

