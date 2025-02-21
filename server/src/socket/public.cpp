#include <string>
#include <cstring>
#include <sys/socket.h>

#include "./socket.h"
#include "../logger/logger.h"

TSPSocket::TSPSocket(const char* host_, const char* port_, const TCPSocketOpts& opts) {
    this->host = host_;
    this->port = port_;
    this->timeout = opts.timeout ? opts.timeout : 0;
    this->backlog = opts.backlog ? opts.backlog : 5;
    this->socket_type = opts.type != "" ? opts.type : "";
    this->users = {};
    for (auto& type_ : this->callback_types)
        this->callback_on[type_] = [](int, std::string) -> int {return 0;};
}

void TSPSocket::start() {
    if (this->socket_type == "server") {
        this->start_listening();
        while (true) this->wait_for_connection();

    } else if (this->socket_type == "client") {
        this->try_to_connect();
        logger("Connection was established");
        this->establish_connection();
    }
}

void TSPSocket::send_msg(int socket, const std::string& msg) {
    send(socket, msg.c_str(), msg.size(), 0);
    this->log_date(socket, "SEND", msg);
}

void TSPSocket::send_msg(int socket, const std::vector<std::string>& msgs) {
    for (std::string msg : msgs) {
        send(socket, msg.c_str(), msg.size(), 0);
        this->log_date(socket, "SEND", msg);
    }
}

void TSPSocket::on(const std::string& type, const OnCallbackStruct& callback) {
    if (type == "connection" || this->socket_type == "server")
        this->create("server");
    else if (type == "open" || this->socket_type == "client")
        this->create("client");

    if (this->callback_on.find(type) != this->callback_on.end())
        this->callback_on[type] = callback;
}

void TSPSocket::send_all(int socket, const std::string& msg) {
    for (auto& user : this->users) {
        if (user.get_socket() == socket)
            this->send_msg(socket, msg);
        this->send_msg(user.get_socket(), (char *)msg.c_str());
    }
}

void TSPSocket::shutdown_server() {
    this->close_users();
    this->close_socket();
}
