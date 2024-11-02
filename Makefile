all: create server client

create:
	@[ -d ./build ] || (mkdir ./build && echo "Directory './build' created.")

server: 
	@echo "Compiling server..."
	@g++\
		./src/server.cpp\
		./src/logger/logger.cpp\
		./src/socket/public.cpp\
		./src/socket/private.cpp\
		./src/socket/user/user.cpp\
		-o ./build/server -pthread -std=c++2a

client: 
	@echo "Compiling client..."
	@g++\
		./src/client.cpp\
		./src/logger/logger.cpp\
		./src/socket/public.cpp\
		./src/socket/private.cpp\
		./src/socket/user/user.cpp\
		-o ./build/client -pthread -std=c++2a

clean:
	@echo "Cleaning..."
	@rm -f ./build/*
