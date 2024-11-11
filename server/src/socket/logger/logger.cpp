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

void logger(std::string log_level, std::string msg, std::string type) {
	std::string log = "[" + type + "] " + msg + '\n';
	if (type == "ERROR") {
		std::cerr << log;
		return;
	}
	if (loggers[type] <= loggers[log_level])
		std::cout << log;
}
