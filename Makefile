BIN=main

SRC_DIR=src
INC_DIR=inc
OBJ_DIR=obj

PY_OUT_DIR=pyobjet

OUT_DIR=$(OBJ_DIR)/$(SRC_DIR)

CC=clang++
CFLAGS=-std=c++11 -O3 -fPIC -Wall -I$(INC_DIR)/ -I.

LIBS=-lGLEW -lGLU -lGL -lglfw -lassimp -lfreeimage -lpthread

SRCS=OBJET.cpp $(wildcard $(SRC_DIR)/*.c*)
OBJS=$(addprefix $(OBJ_DIR)/, $(patsubst %.cpp, %.o, $(SRCS)))


default:
	$(CC) $(CFLAGS) $(LIBS) -o $(BIN) $(SRCS) $(BIN).cpp

python: $(OUT_DIR) $(OBJS)
	swig -python -c++ -outdir $(PY_OUT_DIR) OBJET.i
	$(CC) -c -g $(CFLAGS) `pkg-config --cflags --libs python3` $(LIBS) OBJET.cpp OBJET_wrap.cxx
	$(CC) -shared $(CFLAGS) `pkg-config --cflags --libs python3` $(OBJS) $(LIBS) OBJET_wrap.o -o $(PY_OUT_DIR)/_OBJET.so

$(OBJ_DIR)/%.o: %.cpp
	$(CC) -c -g $(CFLAGS) -o $@ $<

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

clean:
	rm -rf $(BIN) $(OBJ_DIR) OBJET_wrap.cxx OBJET_wrap.o _OBJET.so OBJET.o __pycache__
	rm -rf $(PY_OUT_DIR)/__pycache__ $(PY_OUT_DIR)/OBJET.py $(PY_OUT_DIR)/_OBJET.so
