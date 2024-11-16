#pragma once

#include <map>
#include <string>
#include <sqlite3.h>
#include <vector>
#include "../../lib/json.hpp"

using DBDataStruct = std::map<std::string, std::string>;
using DBDataListStruct = std::vector<DBDataStruct>;

struct StatusStruct {
	int code;
	std::string status;
};

struct DBResponseStruct {
	StatusStruct status;
	DBDataListStruct data;
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
	DBResponseStruct get_data(std::string table);

	/**
	 * @brief Get specific data from a table
	 * @param table (string)
	 * @param id (string)
	 * @return (DBResponseStruct)
	 */
	DBResponseStruct get_data(std::string table, std::string id);

	/**
	 * @brief Push data to a table
	 * @param table (string)
	 * @param data (DBDataListStruct)
	 * @return (DBResponseStruct)
	 */
	DBResponseStruct insert_data(std::string table, DBDataListStruct data);

	/**
	 * @brief Update particular data in a table
	 * @param table (string)
	 * @param data (DBDataListStruct)
	 * @return (DBResponseStruct)
	 */
	DBResponseStruct update_data(std::string table, DBDataListStruct value);

	/**
	 * @brief Delete data from a table
	 * @param table (string)
	 * @param id (string)
	 * @return (DBResponseStruct)
	 */
	DBResponseStruct delete_data(std::string table, std::string id);

private:
	sqlite3 *db;
	DBResponseStruct data;

	int execute_sql(std::string sql, bool is_get = false);
};
