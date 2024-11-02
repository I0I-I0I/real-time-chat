all: create main

create:
	@[ -d ./build ] || (mkdir ./build && echo "Directory './build' created.")

main: 
	@echo "Compiling..."
	@g++\
		./src/server.cpp\
		./src/logger/logger.cpp\
		./src/server/public.cpp\
		./src/server/private.cpp\
		./src/server/user/user.cpp\
		-o ./build/server -pthread -std=c++2a

clean:
	@echo "Cleaning..."
	@rm -f ./build/*
