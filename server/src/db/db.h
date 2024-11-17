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
	 * @return (DBResponseStruct)
	 */
	DBResponseStruct get_data(std::string& table);

	/**
	 * @brief Get specific data from a table
	 * @param table (string)
	 * @param id (string)
	 * @return (DBResponseStruct)
	 */
	DBResponseStruct get_data(std::string& table, std::string& id);

	/**
	 * @brief Push data to a table
	 * @param table (string)
	 * @param data (DBDataListStruct)
	 * @return (DBResponseStruct)
	 */
	DBResponseStruct insert_data(std::string& table, DBDataListStruct& data);

	/**
	 * @brief Update particular data in a table
	 * @param table (string)
	 * @param data (DBDataListStruct)
	 * @return (DBResponseStruct)
	 */
	DBResponseStruct update_data(std::string& table, std::string& id, DBDataStruct& data_list);

	/**
	 * @brief Delete data from a table
	 * @param table (string)
	 * @param id (string)
	 * @return (DBResponseStruct)
	 */
	DBResponseStruct delete_data(std::string& table, std::string& id);

private:
	sqlite3 *db;
	DBResponseStruct response;

	int execute_sql(std::string& sql, bool is_get = false);
};
