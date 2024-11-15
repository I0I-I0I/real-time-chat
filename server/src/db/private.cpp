#include <iostream>
#include <string>
#include <sqlite3.h>
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
	this->data.clear();
	char* zErrMsg = 0;

	int rc = sqlite3_exec(
			this->db,
			sql.c_str(),
			is_get ? callback : 0,
			is_get ? &this->data : 0,
			&zErrMsg);

	if (rc != SQLITE_OK) {
		std::cerr << "SQL error: " << zErrMsg << std::endl;
		sqlite3_free(zErrMsg);
		this->data.clear();
		return -1;
	}

	return 0;
}
