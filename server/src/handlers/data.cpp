#include <iostream>
#include <string>
#include "../../lib/json.hpp"
#include "../packet/packet.h"
#include "./handlers.h"

using json = nlohmann::json;

std::string HandlerOn::data(std::string data) {
	json j = json::parse(data);

	std::cout << j["name"] << std::endl;
	std::cout << j["age"] << std::endl;

	std::string response = Packet::create({
		.type = "message",
		.msg = "JSON received\n"
	});
	return response;
}
