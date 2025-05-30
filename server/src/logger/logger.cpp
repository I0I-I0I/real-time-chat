#include <iostream>
#include <string>
#include <map>
#include "../config.h"
#include "./logger.h"

const char* LOG_LEVEL = std::getenv("LOG_LEVEL") ? std::getenv("LOG_LEVEL") : "INFO";

std::map<std::string, int> loggers = {
    { "ALL", 6 },
    { "LOG", 5 },
    { "INFO", 4 },
    { "MSG", 3 },
    { "CONN", 2 },
    { "ERROR", 1 }
};

void logger(std::string msg, std::string type) {
    std::string log = "[" + type + "] " + msg + '\n';
    if (type == "ERROR") {
        std::cerr << log;
        return;
    }
    if (loggers[type] <= loggers[LOG_LEVEL])
        std::cout << log;
}
