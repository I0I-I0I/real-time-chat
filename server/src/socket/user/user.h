#pragma once

#include <functional>
#include <unistd.h>
#include <vector>
#include "../../logger/logger.h"

class User {
private:
	int socket;
	int id;

	void socket_logger(std::string msg, std::string type = "INFO");

public:
	User(int socket_);

	int get_socket();
	int get_id();
	void set_id(int id_);
	void remove();
	bool operator==(const User& other) const;
};

