#include <string>
#include "../packet/packet.h"
#include "./handlers.h"

std::string HandlerOn::other(std::string data) {
	std::string response = Packet::create({
		.type = "error",
		.msg = "Wrong message type"
	});
	return response;
}
