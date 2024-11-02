#include <iostream>
#include <string>
#include "../consts.h"

void logger(std::string msg, std::string type = "INFO") {
	if (type == "ERROR")
		std::cerr << "[" + type + "] " << msg;
	else
		std::cout << "[" + type + "] " << msg;
}

void error_handler(int error_type, std::string extra_msg = "", bool flag = true) {
	std::string msg;
	switch (error_type) {
		case -1:
			msg = "HI";
			break;
		case ERROR_SOCKET:
			msg = "server: socket";
			break;
		case ERROR_SETSOCKOPT:
			msg = "server: setsockopt";
			break;
		case ERROR_GET_ADDR:
			msg = "server: getaddrinfo: ";
			break;
		case ERROR_BIND:
			msg = "server: on binding";
			break;
		case ERROR_LISTEN:
			msg = "server: failed on listen";
			break;
		case ERROR_ACCEPT:
			msg = "server: accept";
			break;
		case ERROR_WRONG_TYPE_FOR_ON:
			msg = "You are dummy ('on' wrong type)\n";
			break;
		case ERROR_CONNECT:
			msg = "Connect failed";
			break;
		default:
			msg = "How did I get here?\n";
			msg = "Error code: ";
	}
	msg += extra_msg;
	logger(msg, "ERROR");
	if (flag)
		exit(error_type);
}
