#include <string>
#include "../packet/packet.h"
#include "./handlers.h"

std::string HandlerOn::close(std::string data) {
	std::string response = Packet::create({
		.type = "close",
		.msg = "BYE!\n"
	});
	return response;
}
