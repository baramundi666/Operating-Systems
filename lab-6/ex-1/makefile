CC = gcc
CFLAGS = -std=c17 -Wall
BUILD_DIR = build

.PHONY: all integral test clean

all:
	make integral

integral: integral.c
	mkdir -pv $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

test:
	./$(BUILD_DIR)/integral 0.001 1000
	./$(BUILD_DIR)/integral 0.001 200
	./$(BUILD_DIR)/integral 0.001 50
	./$(BUILD_DIR)/integral 0.0000001 5
	./$(BUILD_DIR)/integral 0.0000001 500
	./$(BUILD_DIR)/integral 0.0000001 1000
	./$(BUILD_DIR)/integral 0.0000001 5
	./$(BUILD_DIR)/integral 0.00000001 500
	./$(BUILD_DIR)/integral 0.00000001 1000

clean:
	rm -rf $(BUILD_DIR)