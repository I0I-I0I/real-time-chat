#include <iostream>
#include <sqlite3.h>
#include "./db.h"

DB::DB(std::string path) {
	int rc = sqlite3_open(path.c_str(), &this->db);
	if (rc)
		std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
}

DBDataListStruct DB::get_data(std::string table) {
	std::string sql = "SELECT * FROM " + table;
	this->execute_sql(sql, true);
	return this->data;
}

DBDataStruct DB::get_data_by_id(std::string table, std::string id) {
	std::string sql = "SELECT * FROM " + table + " WHERE id = " + id;
	this->execute_sql(sql, true);
	return this->data[0];
}

int DB::insert_data(std::string table, DBDataListStruct data_list) {
    if (data_list.empty()) return 0;

	std::string sql;
    for (const auto& row : data_list) {
		sql = "INSERT INTO " + table + " (";

		for (auto it = row.begin(); it != row.end(); ++it)
			sql += it->first + ", ";
		sql = sql.substr(0, sql.size() - 2) + ") VALUES (";
        for (auto it = row.begin(); it != row.end(); ++it)
            sql += "'" + it->second + "', ";
        sql = sql.substr(0, sql.size() - 2) + ")";

		if (this->execute_sql(sql) != 0) return -1;
    }

    return 0;
}

int DB::update_data(std::string table, DBDataListStruct data_list) {
	for (const auto& row : data_list) {
		std::string sql = "UPDATE " + table + " SET ";
		for (auto it = row.begin(); it != row.end(); ++it) {
			if (it->first == "id") continue;
			sql += it->first + " = '" + it->second + "', ";
		}
		sql = sql.substr(0, sql.size() - 2);
		sql += " WHERE id = " + row.at("id");
		this->execute_sql(sql);
	}
	return 0;
}

int DB::delete_data(std::string table, std::string id) {
	std::string sql = "DELETE FROM " + table + " WHERE id = " + id;
	this->execute_sql(sql);
	return 0;
}
