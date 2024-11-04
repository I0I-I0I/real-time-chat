#pragma once

#include <functional>
#include <winsock2.h>
#include <unistd.h>
#include <vector>
#include "../logger/logger.h"

class User {
private:
	SOCKET socket;
	int id;

public:
	User(SOCKET socket_ = 0);

	SOCKET get_socket();
	int get_id();
	void set_id(int id_);
	void remove();
	bool operator==(const User& other) const;
};

