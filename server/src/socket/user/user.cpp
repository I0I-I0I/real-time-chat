#include <cstdlib>
#include <random>
#include <sstream>
#include <unistd.h>
#include "../../logger/logger.h"
#include "./user.h"

void User::generate_uuid() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);
    std::uniform_int_distribution<> dis_version(0, 0xF);

    std::ostringstream oss;
    int i;
    for (i = 0; i < 8; ++i)
        oss << std::hex << dis(gen);
    oss << "-";
    for (i = 0; i < 4; ++i)
        oss << std::hex << dis(gen);
    oss << "-4";
    for (i = 0; i < 3; ++i)
        oss << std::hex << dis(gen);
    oss << "-";
    oss << std::hex << ((dis(gen) & 0x3F) | 0x80);
    for (i = 0; i < 3; ++i)
        oss << std::hex << dis(gen);
    oss << "-";
    for (i = 0; i < 12; ++i)
        oss << std::hex << dis(gen);

    this->id = oss.str();
}

User::User(int socket_) {
    this->socket = socket_;
    this->generate_uuid();
}

void User::remove() {
    close(this->socket);
    this->socket_logger("User " + this->id + " disconnected", "CONN");
}

bool User::operator==(const User& other) const {
    return this->socket == other.socket;
}

std::string User::get_id() {
    return this->id;
}

int User::get_socket() {
    return this->socket;
}

void User::socket_logger(std::string msg, std::string type) {
    logger(msg, type);
}
