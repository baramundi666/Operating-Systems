CC = gcc
CFLAGS = -std=c17 -Wall -g -O0
BUILD_DIR = build

.PHONY: all collatz_static collatz_shared clean

all:
	make collatz_static
	make collatz_shared

collatz_static: collatz_library.c
	mkdir -pv $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $(BUILD_DIR)/$@.o
	ar rcs $(BUILD_DIR)/lib$@.a $(BUILD_DIR)/$@.o

collatz_shared: collatz_library.c
	mkdir -pv $(BUILD_DIR)
	$(CC) $(CFLAGS) -fPIC -c $< -o $(BUILD_DIR)/$@.o
	$(CC) -shared -o $(BUILD_DIR)/lib$@.so $(BUILD_DIR)/$@.o -lc

clean:
	rm -rf $(BUILD_DIR)