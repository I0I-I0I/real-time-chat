#pragma once

#include <any>
#include <functional>
#include <map>
#include <string>
#include <vector>
#include "./user/user.h"

#define BUFFER_SIZE 2048

constexpr const int CLOSE_CONNECTION = -101;

using OnCallbackStruct = std::function<void(int, const std::any&)>;

/**
 * @param backlog (unsigned int)
 * @param timeout (unsigned int)
 */
struct SocketOpts {
	unsigned int backlog;
	unsigned int timeout;
};

/**
 * @param type (string)
 * @param msg (string)
 */
struct DataStruct {
	std::string type;
	std::string msg;
};

class Socket {
private:
	const char* host;
	const char* port;
	unsigned int backlog;
    unsigned int timeout;
	int main_socket;
	struct addrinfo* addr;
	std::string socket_type;
	std::string buffer;

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
	User get_current_user(int &socket);
	void remove_user(User& user);

	void connection_handler();
	void connection_handler(User user);

	void close_users();
	void close_socket();

	void log_date(int &socket, std::string log_type, std::string msg);
	void error_handler(int error_type, std::string extra_msg = "", bool flag = true);
	void socket_logger(std::string msg, std::string type = "INFO");

public:
	/**
	 * @brief Create socket
	 * @param host (const char*)
	 * @param port (const char*)
	 * @param opts (SocketOpts) = { backlog = 5, timeout = 3000 }
	 */
	Socket(const char* host, const char* port, const SocketOpts& opts = {});

	/**
	 * @brief Run socket
	 */
	void start();

	/**
	 * @brief response on message by type
	 * @param type (string): "connection"(server only) or "open"(client only), "close", "*" or custom data type
	 * @param callback (OnCallbackStruct): function(int socket, std::string info)
	 */
	void on(const std::string& type, const OnCallbackStruct& callback);

	/**
	 * @brief Handle received data. After than, the data is passed to on_callback function with a specific type
	 * @param socket (const int)
	 * @param type (const string)
	 * @param data (const any)
	 */
	void handle_received_data(int socket, const std::string& type, const std::any& data);

	/**
	 * @brief Send message
	 * @param socket (int)
	 * @param msg (const string)
	 */
	void send_msg(int socket, const std::string& msg);

	/**
	 * @brief Send many messages
	 * @param socket (int)
	 * @param msgs (const vector<string>)
	 */
    void send_msg(int socket, const std::vector<std::string>& msgs);

	/**
	 * @brief Send message to all users
	 * @param socket (int)
	 * @param msg (const string)
	 */
	void send_all(int socket, const std::string& msg);

	/**
	 * @brief Receive message
	 * @param socket (int)
	 * @return (string)
	 */
	std::string receive_msg(int socket);

	/**
	 * @brief Close all connections and stop server
	 */
	void shutdown_server();
};
