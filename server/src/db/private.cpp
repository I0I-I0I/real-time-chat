#include <string>
#include <sqlite3.h>
#include <vector>
#include "../logger/logger.h"
#include "./db.h"

int callback(void *data, int length, char **args, char **col_name) {
	DBDataStruct row;
	for (int i = 0; i < length; i++) {
		std::string col = col_name[i];
		std::string arg = args[i] ? args[i] : "NULL";
		row[col] = arg;
	}
	((std::vector<DBDataStruct>*)data)->push_back(row);
	return 0;
}

int DB::execute_sql(std::string sql, bool is_get) {
	this->data.data.clear();
	char* zErrMsg = 0;

	this->data.status = "200 OK";

	int rc = sqlite3_exec(
		this->db,
		sql.c_str(),
		is_get ? callback : 0,
		is_get ? &this->data.data : 0,
		&zErrMsg
	);

	if (rc != SQLITE_OK) {
		std::string error = "SQL: " + std::string(zErrMsg);
		logger(error, "ERROR");
		sqlite3_free(zErrMsg);
		this->data.status = "400 Bad Request";
		this->data.data.clear();
		this->data.data.push_back({
			{ "status", "Bad request" },
			{ "message", error}
		});
		return -1;
	}

	if (!is_get)
		this->data.data.push_back({
			{ "status", "OK" },
			{ "message", "SQL executed successfully" }
		});

	return 0;
}
