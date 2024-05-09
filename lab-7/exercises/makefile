CC = gcc
CFLAGS = -std=c17 -Wall
BUILD_DIR = build

.PHONY: all posix_queue system_v_queue test clean

all:
	make posix_queue
	make system_v_queue

posix_queue: posix_queue.c
	mkdir -pv $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

system_v_queue: system_v_queue.c
	mkdir -pv $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

test:
	./$(BUILD_DIR)/system_v_queue
	./$(BUILD_DIR)/posix_queue


clean:
	rm -rf $(BUILD_DIR)