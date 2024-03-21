CC = gcc
CFLAGS = -std=c17 -Wall
BUILD_DIR = build

.PHONY: all reverse_1 reverse_1024 clean

all:
	make reverse
	make test

reverse_1: reverse.c
	mkdir -pv $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/reverse -D VER_1

reverse_1024: reverse.c
	mkdir -pv $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/reverse

clean:
	rm -rf $(BUILD_DIR)