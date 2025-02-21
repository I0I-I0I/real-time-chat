#pragma once

#include <string>

class User {
private:
    int socket;
    std::string id;

    void socket_logger(std::string msg, std::string type = "INFO");
    void generate_uuid();

public:
    /**
     * @param socket_ (int)
     */
    User(int socket_);

    /**
     * @brief Get the current socket
     * @return (int)
     */
    int get_socket();

    /**
     * @brief Get the current id
     * @return (string)
     */
    std::string get_id();

    /**
     * @brief Set the id
     * @param id_
     */
    void set_id(int id_);

    /**
     * @brief Remove the user
     */
    void remove();

    bool operator==(const User& other) const;
};

