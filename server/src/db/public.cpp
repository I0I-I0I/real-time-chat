#include <sqlite3.h>
#include "../logger/logger.h"
#include "./db.h"

DB::DB(std::string path) {
	if (sqlite3_open(path.c_str(), &this->db))
		logger("Can't open database: " + std::string(sqlite3_errmsg(db)), "ERROR");
}

DBResponseStruct DB::get_data(std::string& table) {
	std::string sql = "SELECT * FROM " + table;
	if (this->execute_sql(sql, true) != 0) return this->data;

	this->data.status = 200;
	return this->data;
}

DBResponseStruct DB::get_data(std::string& table, std::string& id) {
	if (id == "") return this->get_data(table);
	std::string sql = "SELECT * FROM " + table + " WHERE id = " + id;
	if (this->execute_sql(sql, true) != 0) return this->data;

	this->data.status = 200;
	return this->data;
}

DBResponseStruct DB::insert_data(std::string& table, DBDataListStruct& data_list) {
	std::string sql;
    for (const auto& row : data_list) {
		sql = "INSERT INTO " + table + " (";

		for (auto it = row.begin(); it != row.end(); ++it)
			sql += it.key() + ", ";
		sql = sql.substr(0, sql.size() - 2) + ") VALUES (";
        for (auto it = row.begin(); it != row.end(); ++it)
            sql += "'" + std::string(it.value()) + "', ";
        sql = sql.substr(0, sql.size() - 2) + ")";

		if (this->execute_sql(sql) != 0) return this->data;
    }

	this->data.data.clear();
	this->data.status = 200;
	this->data.data.push_back({
		{ "status", "OK" },
		{ "message", "SQL: Inserted successfully" }
	});

    return this->data;
}

DBResponseStruct DB::update_data(std::string& table, std::string& id, DBDataStruct& data_list) {
	std::string sql = "UPDATE " + table + " SET ";

	for (auto it = data_list.begin(); it != data_list.end(); ++it) {
		if (it.key() == "id") continue;
		sql += std::string(it.key()) + " = '" + std::string(it.value()) + "', ";
	}
	sql = sql.substr(0, sql.size() - 2);
	sql += " WHERE id = " + id;
	if (this->execute_sql(sql) != 0) return this->data;

	this->data.data.clear();
	this->data.status = 200;
	this->data.data.push_back({
		{ "status", "OK" },
		{ "message", "SQL: Updated successfully" }
	});

	return this->data;
}

DBResponseStruct DB::delete_data(std::string& table, std::string& id) {
	std::string sql = "DELETE FROM " + table + " WHERE id = " + id;
	if (this->execute_sql(sql) != 0) return this->data;

	this->data.data.clear();
	this->data.status = 200;
	this->data.data.push_back({
		{ "status", "OK" },
		{ "message", "SQL: Deleted successfully" }
	});

	return this->data;
}