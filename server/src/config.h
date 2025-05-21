#pragma once

#include <cstdlib>
#include <cstring>

extern const char* LOG_LEVEL;
constexpr const bool SPA_MODE = true;

constexpr const char* DEFAULT_DB_PATH = "/home/i0i/code/work/real-time-chat/db/db.sqlite3";
constexpr const char* DEFAULT_FRONTEND_PATH = "/home/i0i/code/work/real-time-chat/frontend/dist/";

inline const char* getFrontendPath() {
    const char* envPath = std::getenv("FRONTED_PATH");
    if (envPath && std::strlen(envPath) > 0) {
        return envPath;
    }
    return DEFAULT_FRONTEND_PATH;
}

inline const char* getDbPath() {
    const char* envPath = std::getenv("DB_PATH");
    if (envPath && std::strlen(envPath) > 0) {
        return envPath;
    }
    return DEFAULT_DB_PATH;
}
