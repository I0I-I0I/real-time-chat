#pragma once

constexpr const bool DEBUG_MODE = true;

constexpr const int NORMAL = 0;
constexpr const int EXIT = 1;
constexpr const int SEND_MASSAGE = 2;

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

constexpr const int WRONG_COMMAND = -21;
constexpr const int ERROR_NO_ARGS = -22;
