CXX = g++
CXXFLAGS = -pthread -std=c++2a

SRCS_HANDLERS = src/handlers/*.cpp

SRCS_PACKET = src/packet/packet.cpp

SRCS_SOCKET = src/socket/logger/logger.cpp \
			  src/socket/logger/error_handler.cpp \
			  src/socket/public.cpp \
			  src/socket/private.cpp \
			  src/socket/user/user.cpp

SRCS = $(SRCS_SOCKET) \
	   $(SRCS_HANDLERS) \
	   $(SRCS_PACKET)


SERVER_SRCS = src/server.cpp \
			  $(SRCS)

CLIENT_SRCS = src/client.cpp \
			  $(SRCS)

BUILD_DIR = build
SERVER_EXEC = $(BUILD_DIR)/server
CLIENT_EXEC = $(BUILD_DIR)/client

all: create server

create:
	@[ -d $(BUILD_DIR) ] || (mkdir $(BUILD_DIR) && echo "Directory $(BUILD_DIR) created")

server: $(SERVER_SRCS)
	@echo "Compiling server..."
	@$(CXX) $(CXXFLAGS) $^ -o $(SERVER_EXEC)
	@clear
	@$(SERVER_EXEC)

client: $(CLIENT_SRCS)
	@echo "Compiling client..."
	@$(CXX) $(CXXFLAGS) $^ -o $(CLIENT_EXEC)

clean:
	@echo "Cleaning..."
	@rm -f $(BUILD_DIR)/*

.PHONY: all create server client clean
