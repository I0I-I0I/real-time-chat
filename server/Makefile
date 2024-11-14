CXX = g++
CXXFLAGS = -pthread -std=c++2a
CXXFLAGSEND = -lsqlite3

SRCS_HANDLERS = src/handlers/*.cpp
SRCS_HTTP = src/http/*.cpp
SRCS_PACKET = src/packet/*.cpp
SRCS_DB = src/db/*.cpp
SRCS_SOCKET = src/socket/logger/logger.cpp \
			  src/socket/logger/error_handler.cpp \
			  src/socket/public.cpp \
			  src/socket/private.cpp \
			  src/socket/user/user.cpp

SRCS = $(SRCS_SOCKET) \
	   $(SRCS_HANDLERS) \
	   $(SRCS_PACKET) \
	   $(SRCS_HTTP) \
	   $(SRCS_DB)

SERVER_SRCS = src/server.cpp \
			  $(SRCS)

CLIENT_SRCS = src/client.cpp \
			  $(SRCS)

BUILD_DIR = build
SERVER_EXEC = $(BUILD_DIR)/server

all: create server start

create:
	@[ -d $(BUILD_DIR) ] || (mkdir $(BUILD_DIR) && echo "Directory $(BUILD_DIR) created")

start:
	@clear
	@$(SERVER_EXEC)

server: $(SERVER_SRCS)
	@echo "Compiling server..."
	@$(CXX) $(CXXFLAGS) $^ -o $(SERVER_EXEC) $(CXXFLAGSEND)

clean:
	@echo "Cleaning..."
	@rm -f $(BUILD_DIR)/*

.PHONY: all create server client clean
