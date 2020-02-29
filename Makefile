BIN=main

SRC_DIR=src/
INC_DIR=inc/

CC=clang++
CFLAGS= -std=c++11 -Wall -I$(INC_DIR)

LIBS=-lglfw -lGL -lX11 -lassimp -lpthread -lXrandr -lXi -ldl

SRCS=$(BIN).cpp $(wildcard $(SRC_DIR)*.c*)

default:
	$(CC) $(CFLAGS) $(LIBS) -o $(BIN) $(SRCS)

clean:
	rm -rf $(BIN)
