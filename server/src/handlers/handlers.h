#pragma once

#include <string>

class HandlerOn {
public:
	static std::string connection(std::string data);
	static std::string message(std::string data);
	static std::string all(std::string data);
	static std::string data(std::string data);
	static std::string error(std::string data);
	static std::string other(std::string data);
	static std::string close(std::string data);
};
