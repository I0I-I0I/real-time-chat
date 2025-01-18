#pragma once

#include <string>

struct PacketStruct {
	unsigned short length;
	std::string type;
	std::string msg;
};

class Packet {
private:
	std::string type;
	std::string msg;

	static unsigned short get_length(std::string msg);
	static unsigned short char_to_short(char char1, char char2);
	static char* short_to_char(unsigned short number);

public:

    /**
     * @param str_packet (char*)
     * @return (PacketStruct)
     */
	static PacketStruct parce(char* str_packet);

    /**
     * @param packet (PacketStruct)
     * @return (string)
     */
	static std::string create(PacketStruct packet);
};
