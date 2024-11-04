#include <string>
#include "./packet.h"

PacketStrStruct Packet::create(PacketStruct packet) {
	char* len_char = new char[2];
	len_char = short_to_char(get_length(packet.msg) + get_length(packet.type) + 1);

	std::string msg = packet.type + ":" + packet.msg;
	unsigned short total_length = 2 + msg.length();
	char* res = new char[total_length + 1];

	for (int i = 0; i < 2; i++) res[i] = len_char[i];
	for (int i = 0; i < msg.length(); i++) res[i + 2] = msg[i];
	res[total_length + 1] = '\0';

	delete[] len_char;
	return { total_length, res };
}

PacketStruct Packet::parce(char* char_packet) {
	PacketStruct packet;
	int index = 0;

	packet.length = char_to_short(char_packet[0], char_packet[1]);
	for (int i = 2; i < packet.length + 2; i++) {
		if (char_packet[i] == ':') {
			index++;
			continue;
		}
		else if (index == 0) packet.type += char_packet[i];
		else if (index == 1) packet.msg += char_packet[i];
	}

	return packet;
}

unsigned short Packet::get_length(std::string msg) {
	unsigned short len = msg.length();
	return len;
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
