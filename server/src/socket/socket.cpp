#include <string>
#include <thread>
#include <cstring>
#include <algorithm>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "../logger/logger.h"
#include "./user/user.h"
#include "./socket.h"

std::mutex vector_mutex;

/*
 * -- public --
 */

TCPSocket::TCPSocket(const char* host_, const char* port_, const TCPSocketOpts& opts) {
    this->host = host_;
    this->port = port_;
    this->timeout = opts.timeout ? opts.timeout : 0;
    this->backlog = opts.backlog ? opts.backlog : 5;
    this->socket_type = opts.type != "" ? opts.type : "";
    this->users = {};
    for (auto& type_ : this->callback_types)
        this->callback_on[type_] = [](int, std::string) -> int {return 0;};
}

void TCPSocket::start() {
    if (this->socket_type == "server") {
        this->start_listening();
        while (true) this->wait_for_connection();
    } else if (this->socket_type == "client") {
        this->try_to_connect();
        logger("Connection was established");
        this->establish_connection();
    }
}

void TCPSocket::send_msg(int fd, const std::string& msg) {
    send(fd, msg.c_str(), msg.size(), 0);
    this->log_date(fd, "SEND", msg);
}

void TCPSocket::send_msg(int fd, const std::vector<std::string>& msgs) {
    for (std::string msg : msgs) {
        send(fd, msg.c_str(), msg.size(), 0);
        this->log_date(fd, "SEND", msg);
    }
}

void TCPSocket::send_all(int fd, const std::string& msg) {
    for (auto& user : this->users) {
        if (user.get_socket() == fd)
            this->send_msg(fd, msg);
        this->send_msg(user.get_socket(), (char *)msg.c_str());
    }
}

std::string TCPSocket::recv_msg(int fd) {
    char buffer_char[BUFFER_SIZE];
    recv(fd, buffer_char, BUFFER_SIZE, 0);
    this->log_date(fd, "RECV", buffer_char);
    return std::string(buffer_char);
}

void TCPSocket::on(const std::string& type, const OnCallbackStruct& callback) {
    if (type == "connection" || this->socket_type == "server")
        this->create("server");
    else if (type == "open" || this->socket_type == "client")
        this->create("client");

    if (this->callback_on.find(type) != this->callback_on.end())
        this->callback_on[type] = callback;
}

void TCPSocket::shutdown_server() {
    this->close_users();
    this->close_socket();
}

/*
 * -- private --
 */

void* TCPSocket::get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET)
        return &(((struct sockaddr_in*)sa)->sin_addr);
    return  &(((struct sockaddr_in6*)sa)->sin6_addr);
}

struct addrinfo* TCPSocket::get_addr() {
    struct addrinfo hints, *servinfo;
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    // AF_INET - IPv4
    // SOCK_STREAM - TCP
    // AI_FLAGS - for passive mode

    if ((rv = getaddrinfo(this->host, this->port, &hints, &servinfo)) != 0)
        error_handler(ERROR_GET_ADDR, gai_strerror(rv), false);

    return servinfo;
}

void TCPSocket::create(std::string type_) {
    this->socket_type = type_;
    this->addr = this->get_addr();
    this->main_socket = this->setup_socket();
    logger("Socket was created");
}

int TCPSocket::setup_socket() {
    struct addrinfo *p;
    int yes = 1;
    int sock;

    for (p = this->addr; p != NULL; p = p->ai_next) {
        if ((sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            error_handler(ERROR_SOCKET, "", false);
            continue;
        }
        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(yes)) == -1)
            error_handler(ERROR_SETSOCKOPT, "", false);
        if (bind(sock, p->ai_addr, p->ai_addrlen) == -1) {
            close(sock);
            error_handler(ERROR_BIND, "", false);
            continue;
        }
        break;
    }

    if (p == NULL)
        error_handler(ERROR_BIND);

    return sock;
}

void TCPSocket::try_to_connect() {
    char s[INET6_ADDRSTRLEN];

    if (connect(this->main_socket, this->addr->ai_addr, this->addr->ai_addrlen) == -1) {
        error_handler(ERROR_CONNECT);
        close(this->main_socket);
    }

    inet_ntop(this->addr->ai_family, get_in_addr((struct sockaddr *)this->addr->ai_addr), s, sizeof s);
    logger("client: connected to " + (std::string)s + ":" + this->port, "CONN");
}

void TCPSocket::start_listening() {
    if (listen(this->main_socket, this->backlog) == -1)
        error_handler(ERROR_LISTEN);
    logger("Server was started http://" + std::string(this->host) + ":" + std::string(this->port));
}

void TCPSocket::establish_connection() {
    std::string buf = "";
    this->callback_on["open"](this->main_socket, buf);
    while (true) {
        if (this->callback_on["chatting"](this->main_socket, buf) != 0) break;
    }
    this->callback_on["close"](this->main_socket, buf);
    this->close_socket();
    logger("Connection closed", "CONN");
}

void TCPSocket::establish_connection(User user) {
    int fd = user.get_socket();
    std::string buf = "";
    this->callback_on["connection"](fd, buf);
    while (true) {
        buf = this->recv_msg(fd);
        if (buf == "") break;
        if (this->callback_on["chatting"](fd, buf) != 0) break;
    }
    this->callback_on["close"](fd, buf);
    logger("Connection closed", "CONN");
    this->remove_user(user);
    user.remove();
}

int TCPSocket::accept_connection() {
    char s[INET_ADDRSTRLEN];
    int fd;
    socklen_t sin_size;
    struct sockaddr_storage their_addr;

    sin_size = sizeof their_addr;
    if ((fd = accept(this->main_socket, (struct sockaddr *)&their_addr, &sin_size)) == -1)
        error_handler(ERROR_ACCEPT);

    if (this->timeout > 0) {
        struct timeval timeout = {
            .tv_sec = this->timeout,
            .tv_usec = 0
        };
        int info = setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
        if (info < 0) {
            error_handler(ERROR_SETSOCKOPT, "On setup timeout", false);
        }
    }

    inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
    logger("server: got connection from " + (std::string)s + ":" + this->port, "CONN");
    return fd;
}

User TCPSocket::get_connection() {
    User user(this->accept_connection());
    this->safe_add_user(user);
    logger("User " + this->safe_get_id(user) + " connected", "CONN");
    return user;
}

void TCPSocket::wait_for_connection() {
    User user = this->get_connection();
    logger("Count of users: " + std::to_string(this->safe_users_size()), "CONN");
    std::thread ([this, user]() {
        this->establish_connection(user);
        logger("Count of users: " + std::to_string(this->safe_users_size()), "CONN");
    }).detach();
}

User TCPSocket::get_current_user(int& fd) {
    std::lock_guard<std::mutex> lock(vector_mutex);
    for (auto& user : this->users)
        if (user.get_socket() == fd)
            return user;
    logger("User not found", "ERROR");
    return User(-1);
}

void TCPSocket::safe_add_user(User& user) {
    std::lock_guard<std::mutex> lock(vector_mutex);
    this->users.push_back(user);
}

std::string TCPSocket::safe_get_id(User &user) {
    std::lock_guard<std::mutex> lock(vector_mutex);
    return user.get_id();
}

int TCPSocket::safe_users_size() {
    std::lock_guard<std::mutex> lock(vector_mutex);
    return this->users.size();
}

void TCPSocket::remove_user(User& user) {
    std::lock_guard<std::mutex> lock(vector_mutex);
    this->users.erase(
        std::remove(this->users.begin(), this->users.end(), user),
        this->users.end()
    );
}

void TCPSocket::close_users() {
    for (auto& user : this->users)
        user.remove();
}

void TCPSocket::close_socket() {
    close(this->main_socket);
}

void TCPSocket::log_date(int& fd, std::string log_type, std::string msg) {
    std::string log_msg = log_type;
    if (this->socket_type == "server") {
        User current_user = this->get_current_user(fd);
        std::string id = this->safe_get_id(current_user);
        log_msg += "(" + id + "): "+ msg;
    } else {
        log_msg += ": " + msg;
    }
    logger(log_msg, "ALL");
}

void TCPSocket::error_handler(int error_type, std::string extra_msg, bool flag) {
    std::string msg;
    switch (error_type) {
        case -1:
            msg = "HI";
            break;
        case ERROR_SOCKET:
            msg = "server: socket";
            break;
        case ERROR_SETSOCKOPT:
            msg = "server: setsockopt";
            break;
        case ERROR_GET_ADDR:
            msg = "server: getaddrinfo: ";
            break;
        case ERROR_BIND:
            msg = "server: on binding";
            break;
        case ERROR_LISTEN:
            msg = "server: failed on listen";
            break;
        case ERROR_ACCEPT:
            msg = "server: accept";
            break;
        case ERROR_CONNECT:
            msg = "Connect failed";
            break;
        case ERROR_WSA_STARTUP:
            msg = "WSAStartup failed";
            break;
        default:
            msg = "How did I get here?\n";
            msg = "Error code: ";
    }
    msg += " " + extra_msg;
    logger(msg, "ERROR");
    if (flag) {
        exit(error_type);
    }
}
