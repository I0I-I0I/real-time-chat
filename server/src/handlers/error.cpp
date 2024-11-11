#include <string>
#include "../packet/packet.h"
#include "./handlers.h"

std::string HandlerOn::error(std::string data) {
	std::string response = Packet::create({
		.type = "message",
		.msg = "ERROR RECEIVED\n"
	});
	return response;
}
