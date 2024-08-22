CC = g++
COMPILER_FLAGS = -std=c++20 -Wall -O0 -g
SRC_FILES = $(wildcard ./*.cpp)
OBJ_NAME = run 

all:
	$(CC) $(COMPILER_FLAGS) $(SRC_FILES) -o $(OBJ_NAME)
