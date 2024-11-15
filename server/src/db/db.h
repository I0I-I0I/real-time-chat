#pragma once

#include <map>
#include <string>
#include <sqlite3.h>
#include <vector>
#include "../../lib/json.hpp"

using DBDataStruct = std::map<std::string, std::string>;
using DBDataListStruct = std::vector<DBDataStruct>;

class DB {
public:
	DB(std::string path);
	~DB() {
		if (this->db) sqlite3_close(this->db);
	};

	/**
	 * @brief Get all data from a table
	 * @param table (string)
	 * @return (DBDataStruct)
	 */
	DBDataListStruct get_data(std::string table);

	/**
	 * @brief Get specific data from a table
	 * @param table (string)
	 * @param id (string)
	 * @return (DBDataStruct)
	 */
	DBDataStruct get_data_by_id(std::string table, std::string id);

	/**
	 * @brief Push data to a table
	 * @param table (string)
	 * @param data (vector<map<string, string>>)
	 * @return (int)
	 */
	int insert_data(std::string table, DBDataListStruct data);

	/**
	 * @brief Update particular data in a table
	 * @param table (string)
	 * @param data (vector<map<string, string>>)
	 * @return (int)
	 */
	int update_data(std::string table, DBDataListStruct value);

	/**
	 * @brief Delete data from a table
	 * @param table (string)
	 * @param id (string)
	 * @return (int)
	 */
	int delete_data(std::string table, std::string id);

	nlohmann::json to_json() const;

private:
	sqlite3 *db;
	DBDataListStruct data;

	int execute_sql(std::string sql, bool is_get = false);
};
