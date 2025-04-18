CXX = clang++
CXXFLAGS = -std=c++2a -Wall -Wextra -Wpedantic -Werror
CXXFLAGSEND = -lsqlite3 -pthread -lssl -lcrypto -I./libs
CXXOPTIMIZATIONS = -O3 -flto

SRCS_HTTP = src/http/*.cpp
SRCS_PACKET = src/packet/*.cpp
SRCS_DB = src/db/*.cpp \
		  src/db/utils/*.cpp
SRCS_LOGGER = src/logger/*.cpp
SRCS_SOCKET = src/socket/*.cpp \
			  src/socket/user/*.cpp
SRCS_HANDLERS = src/handlers/options/*.cpp \
				src/handlers/get/*.cpp \
				src/handlers/post/*.cpp \
				src/handlers/put/*.cpp \
				src/handlers/del/*.cpp \
				src/handlers/utils/*.cpp

SRCS = $(SRCS_SOCKET) \
	   $(SRCS_HANDLERS) \
	   $(SRCS_PACKET) \
	   $(SRCS_HTTP) \
	   $(SRCS_LOGGER) \
	   $(SRCS_DB)

SERVER_SRCS = src/main.cpp \
			  $(SRCS)

BUILD_DIR = build
SERVER_EXEC = $(BUILD_DIR)/server
SERVER_BUILD_EXEC = ../build/server

all: create server

create:
	@[ -d $(BUILD_DIR) ] || (mkdir $(BUILD_DIR) && echo "Directory $(BUILD_DIR) created")

run: create server
	@clear
	@$(SERVER_EXEC)

server: $(SERVER_SRCS)
	@echo "Compiling server..."
	@$(CXX) $(CXXFLAGS) $^ -o $(SERVER_EXEC) $(CXXFLAGSEND)

for_build: $(SERVER_SRCS)
	@echo "Compiling build version of server..."
	@$(CXX) $(CXXFLAGS) $(CXXOPTIMIZATIONS) $^ -o $(SERVER_EXEC) $(CXXFLAGSEND) -L/usr/lib

debug: $(SERVER_SRCS)
	@echo "Compiling server (for debug)..."
	@$(CXX) $(CXXFLAGS) $^ -o $(SERVER_EXEC) $(CXXFLAGSEND) -g

clean:
	@echo "Cleaning..."
	@rm -f $(BUILD_DIR)/*

.PHONY: all create server clean
