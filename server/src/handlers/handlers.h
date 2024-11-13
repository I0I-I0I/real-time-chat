#pragma once

#include <any>
#include <string>
#include "../http/http.h"

class HandlerOn {
public:
	static std::string get(const std::any& data);
	static std::string post(const std::any& data);
};
