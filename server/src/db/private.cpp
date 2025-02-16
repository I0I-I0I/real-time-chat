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

int DB::execute_sql(std::string& sql, bool is_get) {
    this->response.body.data.clear();
    char* zErrMsg = 0;

    int rc = sqlite3_exec(
        this->db,
        sql.c_str(),
        is_get ? callback : 0,
        is_get ? &this->response.body.data : 0,
        &zErrMsg
    );

    if (rc != SQLITE_OK) {
        std::string error = "SQL: " + std::string(zErrMsg);
        logger(error, "ERROR");
        sqlite3_free(zErrMsg);
        this->response.status = 400;
        this->response.body.data.clear();
        this->response.body.status = "Bad request";
        this->response.body.msg = error;
        return -1;
    }

    if (is_get && this->response.body.data.empty()) {
        this->response.status = 400;
        this->response.body.data.clear();
        this->response.body.status = "No data";
        this->response.body.msg = "SQL: No data";
        return -2;
    }

    return 0;
}
