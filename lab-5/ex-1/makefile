CC = gcc
CFLAGS = -std=c17 -Wall
BUILD_DIR = build

.PHONY: all signal test clean

all:
	make signal

signal: signal.c
	mkdir -pv $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

test:
	./$(BUILD_DIR)/signal ignore
	./$(BUILD_DIR)/signal handler
	./$(BUILD_DIR)/signal mask
	./$(BUILD_DIR)/signal none

clean:
	rm -rf $(BUILD_DIR)