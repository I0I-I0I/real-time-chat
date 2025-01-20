#pragma once

#include <map>
#include <string>
#include <sqlite3.h>
#include <vector>
#include "../../lib/json.hpp"

using DBDataStruct = nlohmann::json;
using DBDataListStruct = std::vector<DBDataStruct>;

/**
 * @brief Database response body
 * @param status (string)
 * @param msg (string)
 * @param data (DBDataListStruct)
 */
struct DBResponseBodyStruct {
	std::string status;
	std::string msg;
	DBDataListStruct data;
};

/**
 * @brief Database response
 * @param status (int)
 * @param body (DBResponseBodyStruct)
 */
struct DBResponseStruct {
	int status;
	DBResponseBodyStruct body;
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
	 * @return (DBResponseStruct)
	 */
	DBResponseStruct get_data(const std::string& table, const std::string fields = "*");

	/**
	 * @brief Get specific data from a table
     * @param by (string)
	 * @param table (string)
	 * @param id (string)
	 * @param fields (string) = "*"
	 * @return (DBResponseStruct)
	 */
    DBResponseStruct get_data_by(const std::string by, const std::string& table, const std::string& id, const std::string fields = "*");

	/**
	 * @brief Push data to a table
	 * @param table (string)
	 * @param data (DBDataListStruct)
	 * @return (DBResponseStruct)
	 */
	DBResponseStruct insert_data(const std::string& table, DBDataListStruct& data);

	/**
	 * @brief Update particular data in a table
	 * @param table (string)
     * @param id (string)
	 * @param data_list (DBDataListStruct)
	 * @return (DBResponseStruct)
	 */
	DBResponseStruct update_data(const std::string& table, std::string& id, DBDataStruct& data_list);

	/**
	 * @brief Delete data from a table
	 * @param table (string)
	 * @param id (string)
	 * @return (DBResponseStruct)
	 */
	DBResponseStruct delete_data(const std::string& table, std::string& id);

	/**
	 * @brief Check on exists
	 * @param table (string)
	 * @param login (string)
	 * @param password (string)
	 * @param fields (string)
	 * @return (DBResponseStruct)
	 */
	DBResponseStruct check_password(const std::string& table, const std::string& login, const std::string& password, const std::vector<std::string>& fields = {});

private:
	sqlite3 *db;
	DBResponseStruct response;

	int execute_sql(std::string& sql, bool is_get = false);
};
