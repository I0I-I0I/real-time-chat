#pragma once

#include <string>
#include <functional>

/**
 * @param msg (string)
 * @param type (string) = "INFO"
 */
void logger(std::string msg, std::string type = "INFO");

constexpr const int ERROR_ACCEPT = -10;
constexpr const int ERROR_LISTEN = -11;
constexpr const int ERROR_SOCKET = -12;
constexpr const int ERROR_LAUNCH = -13;
constexpr const int ERROR_SETSOCKOPT = -14;
constexpr const int ERROR_BIND = -15;
constexpr const int ERROR_SEND = -16;
constexpr const int ERROR_RECV = -17;
constexpr const int ERROR_GET_ADDR = -18;
constexpr const int ERROR_WRONG_TYPE_FOR_ON = -19;
constexpr const int ERROR_CONNECT = -20;
constexpr const int ERROR_WSA_STARTUP = -21;
