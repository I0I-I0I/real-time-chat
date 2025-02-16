#pragma once

#include <functional>
#include <unistd.h>
#include <vector>
#include "../../logger/logger.h"

class User {
private:
    int socket;
    int id;

    void socket_logger(std::string msg, std::string type = "INFO");

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
     * @return (int)
     */
    int get_id();

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

