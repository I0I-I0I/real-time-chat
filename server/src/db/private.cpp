#include <iostream>
#include <string>
#include <sqlite3.h>
#include "./db.h"

int idx = 0;
std::vector<DBDataStruct> result;

int callback(void *data, int length, char **args, char **col_name) {
	if (idx >= result.size())
        result.resize(idx + 1);
	for (int i = 0; i < length; i++)
		result[idx][col_name[i]] = args[i] ? args[i] : "NULL";
	idx++;
	return 0;
}

int DB::execute_sql(std::string sql, bool is_get) {
	this->data.clear();
	char* zErrMsg = 0;

	int rc = sqlite3_exec(
			this->db,
			sql.c_str(),
			is_get ? callback : 0,
			0,
			&zErrMsg);

	idx = 0;

	if (rc != SQLITE_OK) {
		std::cerr << "SQL error: " << zErrMsg << std::endl;
		sqlite3_free(zErrMsg);
		this->data.clear();
		return -1;
	}

	if (is_get)
		this->data = result;

	return 0;
}
