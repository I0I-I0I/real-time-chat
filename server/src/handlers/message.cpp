#include <string>
#include "../packet/packet.h"
#include "./handlers.h"

std::string HandlerOn::message(std::string data) {
	if (data == "get")
		return Packet::create({
			.type = "json",
			.msg = R"({ "name": "HEMM", "age": 43 })"
		});
	return Packet::create({
		.type = "message",
		.msg = "OK"
	});
}
