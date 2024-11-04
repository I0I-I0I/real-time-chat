#include <iostream>
#include <string>

void logger(std::string msg, std::string type = "INFO") {
	if (type == "ERROR")
		std::cerr << "[" + type + "] " << msg;
	else
		std::cout << "[" + type + "] " << msg;
}
