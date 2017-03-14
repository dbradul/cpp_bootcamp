CC=g++
AR=ar
CFLAGS=--std=c++11 -Wall
BIN_DIR=bin

export $(BIN_DIR)

dir_guard=@mkdir -p $(@D)

.PHONY: main all

all: $(BIN_DIR)/main

subsystem:
	$(MAKE) -C dir1
	$(MAKE) -C dir2

$(BIN_DIR)/%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -I../ -o $@

$(BIN_DIR)/main: subsystem $(BIN_DIR)/main.o dir1/dir1.o
	$(CC) $(BIN_DIR)/main.o ./dir1/dir1.o -o $(BIN_DIR)/main -L../ -lproject

.PHONY: clean
clean:
	rm -rf $(BIN_DIR)
	mkdir $(BIN_DIR)
