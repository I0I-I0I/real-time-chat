#include <string>
#include "../packet/packet.h"
#include "./handlers.h"

std::string HandlerOn::connection(std::string data) {
	std::string response = Packet::create({
			.type = "message",
			.msg = "OK\n"
			});
	return response;
}
