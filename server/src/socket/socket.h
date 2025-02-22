#pragma once

#include <functional>
#include <map>
#include <string>
#include <vector>
#include "./user/user.h"

#define BUFFER_SIZE 2048

constexpr const int CLOSE_CONNECTION = -101;

using OnCallbackStruct = std::function<int(const int&, const std::string&)>;

/**
 * @param backlog (unsigned int)
 * @param timeout (unsigned int)
 */
struct TCPSocketOpts {
    unsigned int backlog;
    unsigned int timeout;
    std::string type;
};

class TCPSocket {
private:
    const char* host;
    const char* port;
    unsigned int backlog;
    unsigned int timeout;
    int main_socket;
    struct addrinfo* addr;
    std::string socket_type;

    std::vector<User> users;
    const std::vector<std::string> callback_types = { "connection", "chatting", "open", "close" };
    std::map<std::string, OnCallbackStruct> callback_on;

    void *get_in_addr(struct sockaddr*);
    struct addrinfo* get_addr();

    int setup_socket();
    void create(std::string type);

    void start_listening();
    int accept_connection();
    void wait_for_connection();
    void try_to_connect();

    User get_connection();
    User get_current_user(int &fd);
    void remove_user(User& user);

    std::string safe_get_id(User &user);

    void establish_connection();
    void establish_connection(User user);

    void safe_add_user(User& user);
    int safe_users_size();

    void close_users();
    void close_socket();

    void log_date(int &fd, std::string log_type, std::string msg);
    void error_handler(int error_type, std::string extra_msg = "", bool flag = true);

public:
    /**
     * @brief Create socket
     * @param host (const char*)
     * @param port (const char*)
     * @param opts (SocketOpts) = { backlog = 5, timeout = 3000 }
     */
    TCPSocket(const char* host, const char* port, const TCPSocketOpts& opts = {});

    /**
     * @brief Run socket
     */
    void start();

    /**
     * @brief response on message by type
     * @param type (string): "connection"(server only) or "open"(client only), "chatting", "close"
     * @param callback (OnCallbackStruct): function(int socket, std::string info)
     */
    void on(const std::string& type, const OnCallbackStruct& callback);

    /**
     * @brief Send message
     * @param socket (int)
     * @param msg (const string)
     */
    void send_msg(int fd, const std::string& msg);

    /**
     * @brief Send many messages
     * @param socket (int)
     * @param msgs (const vector<string>)
     */
    void send_msg(int fd, const std::vector<std::string>& msgs);

    /**
     * @brief Receive message
     * @param socket (int)
     * @return (string)
     */
    std::string recv_msg(int fd);

    /**
     * @brief Send message to all users
     * @param socket (int)
     * @param msg (const string)
     */
    void send_all(int fd, const std::string& msg);

    /**
     * @brief Close all connections and stop server
     */
    void shutdown_server();
};
