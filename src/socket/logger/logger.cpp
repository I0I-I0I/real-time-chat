#include <iostream>
#include <string>
#include <map>
#include "./logger.h"

std::map<std::string, int> loggers = {
	{ "INFO", 4 },
	{ "MSG", 3 },
	{ "CONN", 2 },
	{ "ERROR", 1 }
};

std::function<void(std::string, std::string)> set_log_level(std::string log_level) {
	return [log_level](std::string msg, std::string type = "INFO") -> void {
		if (type == "ERROR") {
			std::cerr << "[" + type + "] " << msg;
			return;
		}
		if (loggers[type] <= loggers[log_level])
			std::cout << "[" + type + "] " << msg;
	};
}

void logger(std::string msg, std::string type) {
	if (type == "ERROR")
		std::cerr << "[" + type + "] " << msg;
	else
		std::cout << "[" + type + "] " << msg;
}
