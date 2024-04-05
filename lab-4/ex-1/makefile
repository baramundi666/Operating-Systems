CC = gcc
CFLAGS = -std=c17 -Wall
BUILD_DIR = build

.PHONY: all spawn test clean

all:
	make spawn
	make test
	make clean

spawn: spawn.c
	mkdir -pv $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

test:
	./build/spawn 5

clean:
	rm -rf $(BUILD_DIR)