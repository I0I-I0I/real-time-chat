#pragma once

#include <string>
#include <sqlite3.h>
#include <vector>
#include "json.hpp"
#include "../http/http.h"

using DBDataStruct = nlohmann::json;
using DBDataListStruct = std::vector<DBDataStruct>;

enum ExecuteType {
    default_,
    get
};

class DB {
public:
    DB(std::string path);
    ~DB() {
        if (this->db) sqlite3_close(this->db);
    };

    /**
     * @brief Get all data from a table
     * @param table (string)
     * @param fields (string) = "*"
     * @return (ResponseDataStruct)
     */
    ResponseDataStruct get_data(const std::string& table, const std::vector<std::string>& fields = { "*" });

    /**
     * @brief Get specific data from a table by value
     * @param by (string)
     * @param table (string)
     * @param value (string)
     * @param fields (string) = "*"
     * @return (ResponseDataStruct)
     */
    ResponseDataStruct get_data_by(
        const std::string by,
        const std::string& table,
        const std::string& value,
        const std::vector<std::string>& fields = { "*" }
    );

    /**
     * @brief Get specific data from a table by many values
     * @param by (string)
     * @param table (string)
     * @param values (vector<string>)
     * @param fields (string) = "*"
     * @return (ResponseDataStruct)
     */
    ResponseDataStruct get_data_by(
        const std::string by,
        const std::string& table,
        std::vector<std::string> values,
        const std::vector<std::string>& fields = { "*" }
    );

    /**
     * @brief Search specific data by value
     * @param by (string)
     * @param table (string&)
     * @param value (string&)
     * @param fields (vector<string>&)
     * @return (ResponseDataStruct)
    */
    ResponseDataStruct search_data(
        const std::string by,
        const std::string& table,
        const std::string& value,
        const std::vector<std::string>& fields
    );

    /**
     * @brief Push data to a table
     * @param table (string)
     * @param data (DBDataListStruct)
     * @return (ResponseDataStruct)
     */
    ResponseDataStruct insert_data(const std::string& table, DBDataListStruct& data, ExecuteType type = ExecuteType::default_);

    /**
     * @brief Update particular data in a table
     * @param table (string)
     * @param id (string)
     * @param data_list (DBDataListStruct)
     * @return (ResponseDataStruct)
     */
    ResponseDataStruct update_data(const std::string& table, std::string id, DBDataStruct& data_list);

    /**
     * @brief Delete data from a table
     * @param table (string)
     * @param id (string)
     * @return (ResponseDataStruct)
     */
    ResponseDataStruct delete_data_by(std::string by,const std::string& table, std::string value);

    /**
     * @brief Check on exists
     * @param table (string)
     * @param login (string)
     * @param password (string)
     * @param fields (string)
     * @return (ResponseDataStruct)
     */
    ResponseDataStruct check_password(const std::string& table, const std::string& login, const std::string& password, const std::vector<std::string>& fields = {});

private:
    sqlite3 *db;
    ResponseDataStruct response;

    int execute_sql(std::string& sql, ExecuteType type = ExecuteType::default_);
};
