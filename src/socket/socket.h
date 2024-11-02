#pragma once

#include <functional>
#include <map>
#include <string>
#include <vector>
#include "./user/user.h"

#define BUFFER_SIZE 1024

class Socket {
private:
	const char* host;
	const char* port;
	int backlog;
	int main_socket;
	struct addrinfo* addr;
	std::string socket_type;

	std::vector<User> users;
	std::vector<std::string> callback_types;
	std::map<std::string, std::function<void(int fd)>> callback_on;

	void *get_in_addr(struct sockaddr*);
	struct addrinfo* get_addr();

	int setup_socket();
	void start_listening();
	int accept_connection();
	void get_connection();
	void connection_handler(User);
	void chating(int socket);

	User wait_for_connection();
	void remove_user(User);

	void close_users();
	void close_socket();

	User get_current_user(int socket);

	void try_connect();

	int setup_socket(std::string type);


public:
	char* buffer = new char[BUFFER_SIZE];

	Socket(const char* host, const char* port, int backlog = 5);

	/**
	 * @brief Setup server
	 * @param type: "client", "server"
	 */
	void create(std::string type);

	/**
	 * @brief Start server
	 */
	void launch();

	/**
	 * @brief Create connection
	 */
	void connection();

	void send_msg(int socket, std::string msg);
	void receive_msg(int socket);
	void ping(int socket, std::string msg);

	/**
	 * @brief response on type
	 * @param type: "connection", "open", "chat", "close"
	 * @param callback: function(int socket)
	 */
	void on(std::string, std::function<void(int socket)>);

	/**
	 * @brief Close all connections and stop server
	 */
	void shutdown_server();
};
