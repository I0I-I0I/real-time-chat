#include <string>
#include <sqlite3.h>
#include <vector>
#include "json.hpp"
#include "../logger/logger.h"
#include "./utils/utils.h"
#include "./db.h"

using json = nlohmann::json;

/*
 * -- public --
 */

DB::DB(const std::string path) {
    if (sqlite3_open(path.c_str(), &this->db))
        logger("Can't open database: " + std::string(sqlite3_errmsg(db)), "ERROR");
}

ResponseDataStruct DB::get_data(const std::string& table, const std::vector<std::string>& fields) {
    std::string fls = join(fields, ", ");
    std::string sql = "SELECT " + fls + " FROM " + table;
    if (this->execute_sql(sql, ExecuteType::get) != 0) return this->response;

    this->response.status = StatusCode::ok;
    this->response.msg = "SQL: OK";
    return this->response;
}

ResponseDataStruct DB::get_data_by(
    const std::string by,
    const std::string& table,
    std::vector<std::string> values,
    const std::vector<std::string>& fields
) {
    std::string fls = join(fields, ", ");

    std::ostringstream oss;
    for (size_t i = 0; i < values.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << "'" << values[i] << "'";
    }
    std::string valuesList = oss.str();

    std::string sql = "SELECT " + fls + " FROM " + table + " WHERE " + by + " IN (" + valuesList + ")";
    if (this->execute_sql(sql, ExecuteType::get) != 0) return this->response;

    this->response.msg = "SQL: OK";
    this->response.status = StatusCode::ok;
    return this->response;
}

ResponseDataStruct DB::get_data_by(
    const std::string by,
    const std::string& table,
    const std::string& value,
    const std::vector<std::string>& fields
) {
    std::string fls = join(fields, ", ");
    std::string sql = "SELECT " + fls + " FROM " + table + " WHERE " + by + " = '" + value + "'";
    if (this->execute_sql(sql, ExecuteType::get) != 0) return this->response;

    this->response.msg = "SQL: OK";
    this->response.status = StatusCode::ok;
    return this->response;
}

ResponseDataStruct DB::search_data(
    const std::string by,
    const std::string& table,
    const std::string& value,
    const std::vector<std::string>& fields
) {
    std::string fls = join(fields, ", ");
    std::string sql = "SELECT " + fls + " FROM " + table + " WHERE (lower(" + by + ") LIKE '%" + value + "%')";
    if (this->execute_sql(sql, ExecuteType::get) != 0) return this->response;

    this->response.msg = "SQL: OK";
    this->response.status = StatusCode::ok;
    return this->response;
}

ResponseDataStruct DB::insert_data(const std::string& table, DBDataListStruct& data_list, ExecuteType type) {
    std::string sql;

    if (type == ExecuteType::get) {
        sql = "BEGIN TRANSACTION;";
        if (this->execute_sql(sql) != 0)
            return this->response;
    }

    for (const auto& row : data_list) {
        sql = "INSERT INTO " + table + " (";

        for (auto it = row.begin(); it != row.end(); ++it)
            sql += it.key() + ", ";
        sql = sql.substr(0, sql.size() - 2) + ") VALUES (";
        for (auto it = row.begin(); it != row.end(); ++it)
            sql += "'" + std::string(it.value()) + "', ";
        sql = sql.substr(0, sql.size() - 2) + ")";

        if (this->execute_sql(sql) != 0)
            return this->response;
    }

    if (type != ExecuteType::get) {
        this->response.data.clear();
        StatusCode status = StatusCode::ok;
        return {
            .status = status,
            .msg = "SQL: Inserted successfully",
            .data = {}
        };
    }

    std::string last_id = std::to_string(sqlite3_last_insert_rowid(this->db));

    sql = "COMMIT;";
    if (this->execute_sql(sql) != 0) {
        sql = "ROLLBACK;";
        this->execute_sql(sql);
        return this->response;
    }
    return this->get_data_by("id", table, last_id);
}

ResponseDataStruct DB::update_data(const std::string& table, std::string id, DBDataStruct& data_list) {
    std::string sql = "UPDATE " + table + " SET ";

    for (auto it = data_list.begin(); it != data_list.end(); ++it) {
        if (it.key() == "id") continue;
        sql += std::string(it.key()) + " = '" + std::string(it.value()) + "', ";
    }
    sql = sql.substr(0, sql.size() - 2);
    sql += " WHERE id = " + id;
    if (this->execute_sql(sql) != 0) return this->response;

    this->response.data.clear();
    StatusCode status = StatusCode::ok;
    return {
        .status = status,
        .msg = "SQL: Updated successfully",
        .data = {}
    };
}

ResponseDataStruct DB::delete_data_by(std::string by,const std::string& table, std::string value) {
    std::string sql = "DELETE FROM " + table + " WHERE " + by + " = '" + value + "'";
    if (this->execute_sql(sql) != 0) return this->response;

    this->response.data.clear();

    StatusCode status = StatusCode::ok;
    return {
        .status = status,
        .msg = "SQL: Deleted successfully",
        .data = {}
    };
}

ResponseDataStruct DB::check_password(const std::string& table, const std::string& login, const std::string& password, const std::vector<std::string>& fields) {
    std::string sql = "SELECT * FROM " + table + " WHERE login = '" + login + "'";
    if (this->execute_sql(sql, ExecuteType::get) != 0)
        return this->response;

    if (this->response.data.at(0)["password"] != password) {
        StatusCode status = StatusCode::wrong_password;
        return {
            .status = status,
            .msg = "Wrong password",
            .data = {}
        };
    }

    std::vector<json> data;
    data.push_back({});
    for (const auto& field : fields) {
        data[0][field] = this->response.data.at(0)[field];
    }

    StatusCode status = StatusCode::ok;
    return {
        .status = status,
        .msg = "Password correct",
        .data = data
    };
}

/*
 * -- private --
 */

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
    this->response.data.clear();
    char* zErrMsg = 0;

    int rc = sqlite3_exec(
        this->db,
        sql.c_str(),
        type != ExecuteType::default_ ? sql_callback : 0,
        type != ExecuteType::default_ ? &this->response.data : 0,
        &zErrMsg
    );

    if (rc != SQLITE_OK) {
        std::string error = "SQL: " + std::string(zErrMsg);
        sqlite3_free(zErrMsg);
        logger(error, "ERROR");
        this->response.status = StatusCode::bad_request;
        this->response.data.clear();
        this->response.msg = error;
        if (error.find("UNIQUE") != std::string::npos) {
            this->response.status = StatusCode::not_unique;
        }
        return -1;
    }

    if (type == ExecuteType::get && this->response.data.empty()) {
        this->response.status = StatusCode::no_data;
        this->response.data.clear();
        this->response.msg = "SQL: No Data";
        return -2;
    }

    return 0;
}
