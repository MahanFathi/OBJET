BIN=main

SRC_DIR=src/
INC_DIR=inc/

CC=clang++
CFLAGS= -std=c++11 -O3 -Wall -I$(INC_DIR)

LIBS=-lGLEW -lGLU -lGL -lglfw -lassimp -lpthread

SRCS=$(BIN).cpp $(wildcard $(SRC_DIR)*.c*)

default:
	$(CC) $(CFLAGS) $(LIBS) -o $(BIN) $(SRCS)

clean:
	rm -rf $(BIN)
