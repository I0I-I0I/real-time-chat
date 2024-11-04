CXX = g++
CXXFLAGS = -pthread -std=c++2a

SRCS =  src/socket/logger/logger.cpp \
		src/socket/logger/error_handler.cpp \
		src/socket/packet/packet.cpp \
		src/socket/public.cpp \
		src/socket/private.cpp \
		src/socket/user/user.cpp

SERVER_SRCS = src/server.cpp $(SRCS)
CLIENT_SRCS = src/client.cpp $(SRCS)

BUILD_DIR = build
SERVER_EXEC = $(BUILD_DIR)/server
CLIENT_EXEC = $(BUILD_DIR)/client

$(shell mkdir -p $(BUILD_DIR))

all: server client

server: $(SERVER_SRCS)
	@echo "Compiling server..."
	@$(CXX) $(CXXFLAGS) $^ -o $(SERVER_EXEC)

client: $(CLIENT_SRCS)
	@echo "Compiling client..."
	@$(CXX) $(CXXFLAGS) $^ -o $(CLIENT_EXEC)

clean:
	@echo "Cleaning..."
	@rm -f $(BUILD_DIR)/*

.PHONY: all create server client clean
