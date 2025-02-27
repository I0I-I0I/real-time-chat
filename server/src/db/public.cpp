#include <sqlite3.h>
#include <iostream>
#include "../logger/logger.h"
#include "./utils/utils.h"
#include "./db.h"

using json = nlohmann::json;

DB::DB(const std::string path) {
    if (sqlite3_open(path.c_str(), &this->db))
        logger("Can't open database: " + std::string(sqlite3_errmsg(db)), "ERROR");
}

DBResponseStruct DB::get_data(const std::string& table, const std::vector<std::string>& fields) {
    std::string fls = join(fields, ", ");
    std::string sql = "SELECT " + fls + " FROM " + table;
    if (this->execute_sql(sql, true) != 0) return this->response;

    this->response.status = StatusCode::ok;
    this->response.body.status = "OK";
    this->response.body.msg = "SQL: OK";
    return this->response;
}

DBResponseStruct DB::get_data_by(
    const std::string by,
    const std::string& table,
    const std::string& value,
    const std::vector<std::string>& fields
) {
    std::string fls = join(fields, ", ");
    std::string sql = "SELECT " + fls + " FROM " + table + " WHERE " + by + " = '" + value + "'";
    if (this->execute_sql(sql, true) != 0) return this->response;

    this->response.body.status = "OK";
    this->response.body.msg = "SQL: OK";
    this->response.status = StatusCode::ok;
    return this->response;
}

DBResponseStruct DB::insert_data(const std::string& table, DBDataListStruct& data_list, bool is_get) {
    std::string sql;


    if (is_get) {
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

    if (!is_get) {
        this->response.body.data.clear();
        return {
            .status = StatusCode::ok,
            .body = {
                .status = "OK",
                .msg = "SQL: Inserted successfully",
                .data = {}
            }
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

DBResponseStruct DB::update_data(const std::string& table, std::string& id, DBDataStruct& data_list) {
    std::string sql = "UPDATE " + table + " SET ";

    for (auto it = data_list.begin(); it != data_list.end(); ++it) {
        if (it.key() == "id") continue;
        sql += std::string(it.key()) + " = '" + std::string(it.value()) + "', ";
    }
    sql = sql.substr(0, sql.size() - 2);
    sql += " WHERE id = " + id;
    if (this->execute_sql(sql) != 0) return this->response;

    this->response.body.data.clear();
    return {
        .status = StatusCode::ok,
        .body = {
            .status = "OK",
            .msg = "SQL: Updated successfully",
            .data = {}
        }
    };
}

DBResponseStruct DB::delete_data(const std::string& table, std::string& id) {
    std::string sql = "DELETE FROM " + table + " WHERE id = " + id;
    if (this->execute_sql(sql) != 0) return this->response;

    this->response.body.data.clear();
    return {
        .status = StatusCode::ok,
        .body = {
            .status = "OK",
            .msg = "SQL: Deleted successfully",
            .data = {}
        }
    };
}

DBResponseStruct DB::check_password(const std::string& table, const std::string& login, const std::string& password, const std::vector<std::string>& fields) {
    std::string sql = "SELECT * FROM " + table + " WHERE login = '" + login + "'";
    if (this->execute_sql(sql, true) != 0)
        return this->response;

    if (this->response.body.data.at(0)["password"] != password) {
        return {
            .status = StatusCode::bad_request,
            .body = {
                .status = "ERROR",
                .msg = "Wrong password",
                .data = {}
            }
        };
    }

    std::vector<json> data;
    data.push_back({});
    for (const auto& field : fields) {
        data[0][field] = this->response.body.data.at(0)[field];
    }

    return {
        .status = StatusCode::ok,
        .body = {
            .status = "OK",
            .msg = "Password correct",
            .data = data
        }
    };
}
