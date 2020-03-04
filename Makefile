BIN=main

SRC_DIR=src
INC_DIR=inc
OBJ_DIR=obj

OUT_DIR=$(OBJ_DIR)/$(SRC_DIR)

CC=clang++
CFLAGS=-std=c++11 -O3 -fPIC -Wall -I$(INC_DIR)/ -I.

LIBS=-lGLEW -lGLU -lGL -lglfw -lassimp -lfreeimage -lpthread

SRCS=OBJET.cpp $(wildcard $(SRC_DIR)/*.c*)
OBJS=$(addprefix $(OBJ_DIR)/, $(patsubst %.cpp, %.o, $(SRCS)))


default:
	$(CC) $(CFLAGS) $(LIBS) -o $(BIN) $(SRCS) $(BIN).cpp

python: $(OUT_DIR) $(OBJS)
	swig -python -c++ OBJET.i
	$(CC) -c -g $(CFLAGS) `pkg-config --cflags --libs python3` $(LIBS) OBJET.cpp OBJET_wrap.cxx
	$(CC) -shared $(CFLAGS) `pkg-config --cflags --libs python3` $(OBJS) $(LIBS) OBJET_wrap.o -o _OBJET.so

$(OBJ_DIR)/%.o: %.cpp
	$(CC) -c -g $(CFLAGS) -o $@ $<

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

clean:
	rm -rf $(BIN) $(OUT_DIR) OBJET_wrap.cxx OBJET_wrap.o _OBJET.so OBJET.o OBJET.py
