CC = gcc
CFLAGS = -std=c17 -Wall
BUILD_DIR = build

.PHONY: all ls_example test clean

all:
	make ls_example
	make test
	make clean

ls_example: ls_example.c
	mkdir -pv $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

test:
	./build/ls_example $(PWD)

clean:
	rm -rf $(BUILD_DIR)