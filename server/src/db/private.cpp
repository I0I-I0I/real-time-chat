#include <string>
#include <sqlite3.h>
#include <vector>
#include "../logger/logger.h"
#include "./db.h"

int sql_callback(void *data, int length, char **args, char **col_name) {
    DBDataStruct row;
    for (int i = 0; i < length; i++) {
        std::string col = col_name[i];
        if (!args[i]) {
            row[col] = nullptr;
            continue;
        }

        try {
            size_t pos;
            std::stoll(args[i], &pos);
            if (pos == strlen(args[i])) {
                row[col] = std::stoll(args[i]);
                continue;
            }

            std::stod(args[i], &pos);
            if (pos == strlen(args[i])) {
                row[col] = std::stod(args[i]);
                continue;
            }
        } catch (...) {}

        row[col] = args[i];
    }
    ((std::vector<DBDataStruct>*)data)->push_back(row);
    return 0;
}

int DB::execute_sql(std::string& sql, ExecuteType type) {
    this->response.body.data.clear();
    char* zErrMsg = 0;

    int rc = sqlite3_exec(
        this->db,
        sql.c_str(),
        type == ExecuteType::get ? sql_callback : 0,
        type == ExecuteType::get ? &this->response.body.data : 0,
        &zErrMsg
    );

    if (rc != SQLITE_OK) {
        std::string error = "SQL: " + std::string(zErrMsg);
        sqlite3_free(zErrMsg);
        logger(error, "ERROR");
        this->response.status = StatusCode::bad_request;
        this->response.body.data.clear();
        this->response.body.status = "Bad request";
        this->response.body.msg = error;
        if (error.find("UNIQUE") != std::string::npos)
            this->response.body.msg = "Not unique";
        return -1;
    }

    if (type == ExecuteType::get && this->response.body.data.empty()) {
        this->response.status = StatusCode::not_found;
        this->response.body.data.clear();
        this->response.body.status = "ERROR";
        this->response.body.msg = "SQL: No data";
        return -2;
    }

    return 0;
}
