#pragma once

#include <string>

struct PacketStruct {
	unsigned short length;
	std::string type;
	std::string msg;
};

struct PacketStrStruct {
	unsigned short length;
	const char* msg;
};

class Packet {
private:
	std::string type;
	std::string msg;

	static unsigned short get_length(std::string msg);
	static unsigned short char_to_short(char char1, char char2);
	static char* short_to_char(unsigned short number);

public:

	static PacketStruct parce(char* str_packet);
	static PacketStrStruct create(PacketStruct packet);
};
