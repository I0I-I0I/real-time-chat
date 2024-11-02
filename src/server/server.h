#pragma once

#include <functional>
#include <map>
#include <string>
#include <vector>
#include "./user/user.h"

#define BUFFER_SIZE 1024

class Server {
private:
	const char* port;
	int server_socket;
	std::vector<User> users;
	int backlog;
	struct addrinfo* addr;
	char* buffer = new char[BUFFER_SIZE];

	std::vector<std::string> callback_types;
	std::map<std::string, std::function<void(int fd, char*)>> callback_on;

	void *get_in_addr(struct sockaddr*);
	struct addrinfo* get_addr();

	int setup_socket();
	void start_listening();
	int accept_connection();
	void get_connection();
	void connection_handler(User);
	void chating(int);

	User wait_for_connection();
	void remove_user(User);

	void close_users();
	void close_server();

public:
	Server(const char* port, int backlog = 5);

	/**
	 * @brief Setup server
	 */
	void create();

	/**
	 * @brief Start server
	 */
	void launch();

	void send_msg(int socket, std::string msg);
	void receive_msg(int socket);

	/**
	 * @brief response on type
	 * @param type: "connection", "message", "error", "close"
	 * @param callback: function(int, char*)
	 */
	void on(std::string, std::function<void(int, char*)>);

	/**
	 * @brief Close all connections and stop server
	 */
	void shutdown_server();
};
