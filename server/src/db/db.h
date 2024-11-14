#pragma once

#include <map>
#include <string>
#include <sqlite3.h>
#include <vector>

using DBDataStruct = std::map<std::string, std::string>;

class DB {
public:
	DB(std::string path);
	~DB() {
		if (this->db) sqlite3_close(this->db);
	};

	/**
	 * @brief Get all data from a table
	 * @param table (string)
	 */
	std::vector<DBDataStruct> get_data(std::string table);

	/**
	 * @brief Get specific data from a table
	 * @param table (string)
	 * @param id (string)
	 */
	DBDataStruct get_data(std::string table, std::string id);

	/**
	 * @brief Push data to a table
	 * @param table (string)
	 * @param data (vector<map<string, string>>)
	 */
	int insert_data(std::string table, std::vector<DBDataStruct> data);

	/**
	 * @brief Update particular data in a table
	 * @param table (string)
	 * @param data (map<string, string>)
	 */
	int update_data(std::string table, std::vector<DBDataStruct> value);

	/**
	 * @brief Delete data from a table
	 * @param table (string)
	 * @param id (string)
	 */
	int delete_data(std::string table, std::string id);

private:
	sqlite3 *db;
	std::vector<DBDataStruct> data;

	int execute_sql(std::string sql, bool is_get = false);
};
