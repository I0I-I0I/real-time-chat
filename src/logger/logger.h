#pragma once

#include <iostream>
#include <string>

void logger(std::string msg, std::string type = "INFO");

void error_handler(int error_type, std::string extra_msg = "", bool flag = true);
