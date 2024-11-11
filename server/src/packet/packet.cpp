#include <cstring>
#include <string>
#include "./packet.h"

std::string Packet::create(PacketStruct packet) {
	unsigned short len = get_length(packet.msg) + get_length(packet.type) + 2;
	len += get_length(std::to_string(len));
	std::string msg = std::to_string(len) + ":" + packet.type + ":" + packet.msg;
	return msg;
}

PacketStruct Packet::parce(char* char_packet) {
	PacketStruct packet;
	int index = 0;

	int idx = 0;
	std::string len = "";
	for (int i = 0; i < 5; i++) {
		idx++;
		if (char_packet[i] == ':') break;
		len += char_packet[i];
	}

	try {
		packet.length = std::stoi(len);
	} catch (std::exception& e) {
		packet.length = 0;
		return packet;
	}

	for (int i = idx; i < packet.length + idx; i++) {
		if (char_packet[i] == ':' && index < 1) {
			index++;
			continue;
		}
		else if (index == 0) packet.type += char_packet[i];
		else if (index == 1) packet.msg += char_packet[i];
	}

	packet.msg += '\n';

	return packet;
}

unsigned short Packet::get_length(std::string msg) {
	return (unsigned short)msg.length();
}

unsigned short Packet::char_to_short(char char1, char char2) {
	unsigned short res = (static_cast<unsigned short>(char1) << 8) | static_cast<unsigned short>(char2);
	return res;
}

char* Packet::short_to_char(unsigned short number) {
	char* res = new char[2];
	char char1 = number >> 8;
	char char2 = number & 0xFF;
	res[0] = char1;
	res[1] = char2;
	return res;
}
