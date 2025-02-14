CXX = g++
CXXFLAGS = -pthread -std=c++2a -Wall
CXXFLAGSEND = -lsqlite3

SRCS_HANDLERS = src/handlers/options/*.cpp \
	src/handlers/get/*.cpp \
	src/handlers/post/*.cpp \
	src/handlers/put/*.cpp \
	src/handlers/del/*.cpp \
	src/handlers/utils/*.cpp

SRCS_HTTP = src/http/*.cpp
SRCS_PACKET = src/packet/*.cpp
SRCS_DB = src/db/*.cpp src/db/utils/*.cpp
SRCS_LOGGER = src/logger/*.cpp
SRCS_SOCKET = src/socket/public.cpp \
			  src/socket/private.cpp \
			  src/socket/user/user.cpp

SRCS = $(SRCS_SOCKET) \
	   $(SRCS_HANDLERS) \
	   $(SRCS_PACKET) \
	   $(SRCS_HTTP) \
	   $(SRCS_LOGGER) \
	   $(SRCS_DB)

SERVER_SRCS = src/server.cpp \
			  $(SRCS)

CLIENT_SRCS = src/client.cpp \
			  $(SRCS)

BUILD_DIR = build
SERVER_EXEC = $(BUILD_DIR)/server

all: create server

create:
	@[ -d $(BUILD_DIR) ] || (mkdir $(BUILD_DIR) && echo "Directory $(BUILD_DIR) created")

run: create server
	@clear
	@$(SERVER_EXEC)

server: $(SERVER_SRCS)
	@echo "Compiling server..."
	@$(CXX) $(CXXFLAGS) $^ -o $(SERVER_EXEC) $(CXXFLAGSEND)

debug: $(SERVER_SRCS)
	@echo "Compiling server (for debug)..."
	@$(CXX) $(CXXFLAGS) $^ -o $(SERVER_EXEC) $(CXXFLAGSEND) -g3

clean:
	@echo "Cleaning..."
	@rm -f $(BUILD_DIR)/*

.PHONY: all create server client clean
