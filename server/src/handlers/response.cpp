#include <iostream>
#include <string>
#include "./handlers.h"

std::string HandlerOn::response(std::string data) {
	std::cout << "HandlerOn::response" << std::endl;
	return data;
}
