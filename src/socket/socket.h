#pragma once

#include <functional>
#include <map>
#include <winsock2.h>
#include <windows.h>
#include <string>
#include <vector>
#include "./user/user.h"
#include "./packet/packet.h"

#define BUFFER_SIZE 1024

constexpr const int CLOSE_CONNECTION = -101;

using OnCallbackStruct = std::function<void(SOCKET, std::string)>;

/**
 * @param int backlog
 * @param int timeout
 */
struct SocketOpts {
	int backlog;
	int timeout;
	std::string log_level;
};

class Socket {
private:
	const char* host;
	const char* port;
	int backlog;
	SOCKET main_socket;
	struct addrinfo* addr;
	std::string socket_type;
	PacketStruct buffer;
	int timeout;
	std::string log_level;

	std::vector<User> users;
	const std::vector<std::string> callback_types = { "connection", "open", "close", "*" };
	std::map<std::string, OnCallbackStruct> callback_on;
	std::map<std::string, OnCallbackStruct> custom_callback_on;

	void *get_in_addr(struct sockaddr*);
	struct addrinfo* get_addr();

	SOCKET setup_socket();
	void create(std::string type);

	void start_listening();
	SOCKET accept_connection();
	void get_connection();
	void try_to_connect();

	User wait_for_connection();
	User get_current_user(SOCKET socket);
	void remove_user(User& user);

	void connection_handler();
	void connection_handler(User user);

	void close_users();
	void close_socket();

	void log_date(SOCKET socket, std::string log_type, std::string msg);
	void socket_logger(std::string msg, std::string type = "INFO");

public:
	/**
	 * @brief Create socket
	 * @param host
	 * @param port
	 * @param opts = { backlog = 5, timeout = 3000 } // defaults
	 */
	Socket(const char* host, const char* port, SocketOpts opts = {});

	/**
	 * @brief Start socket
	 */
	void start();

	/**
	 * @brief response on message by type
	 * @param type: "connection", "open", "close", "*" or custom
	 * @param callback: function(int socket, std::string info)
	 */
	void on(std::string, OnCallbackStruct);

	/**
	 * @brief Send message
	 * @param socket
	 * @param type
	 * @param msg
	 */
	void send_msg(SOCKET socket, std::string type, std::string msg);

	/**
	 * @brief Send message to all users
	 * @param socket
	 * @param type
	 * @param msg
	 */
	void send_all(SOCKET socket, std::string type, std::string msg);

	/**
	 * @brief Receive message
	 * @param socket
	 * @return CLOSE_CONNECTION on (packet.type == "close" || packet.msg == "") or otherwise return 0
	 */
	int receive_msg(SOCKET socket);

	/**
	 * @brief Close all connections and stop server
	 */
	void shutdown_server();
};