#pragma once

#include <functional>
#include <map>
#include <string>
#include <vector>
#include "./user/user.h"
#include "./packet/packet.h"

#define BUFFER_SIZE 1024

constexpr const int CLOSE_CONNECTION = -1;

using OnCallbackStruct = std::function<void(int, std::string)>;

class Socket {
private:
	const char* host;
	const char* port;
	int backlog;
	int main_socket;
	struct addrinfo* addr;
	std::string socket_type;
	PacketStruct _buffer;

	std::vector<User> users;
	const std::vector<std::string> callback_types = { "connection", "open", "close", "*" };
	std::map<std::string, OnCallbackStruct> callback_on;
	std::map<std::string, OnCallbackStruct> custom_callback_on;

	void *get_in_addr(struct sockaddr*);
	struct addrinfo* get_addr();

	int setup_socket();
	void create(std::string type);

	void start_listening();
	int accept_connection();
	void get_connection();
	void try_to_connect();

	User wait_for_connection();
	User get_current_user(int socket);
	void remove_user(User);

	void connection_handler_client();
	void connection_handler_server(User user);

	void close_users();
	void close_socket();

	void log_date(int socket, std::string log_type, std::string msg);

public:
	Socket(const char* host, const char* port, int backlog = 5);

	/**
	 * @brief Start socket
	 */
	void start();

	/**
	 * @brief response on type
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
	void send_msg(int socket, std::string type, std::string msg);

	/**
	 * @brief Send message to all users
	 * @param socket
	 * @param type
	 * @param msg
	 */
	void send_all(int socket, std::string type, std::string msg);

	/**
	 * @brief Receive message
	 * @param socket
	 * @return CLOSE_CONNECTION on packet.type "close" or 0
	 */
	int receive_msg(int socket);

	/**
	 * @brief Close all connections and stop server
	 */
	void shutdown_server();
};
