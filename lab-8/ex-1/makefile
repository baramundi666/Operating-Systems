CC = gcc
CFLAGS = -std=c17 -Wall
BUILD_DIR = build

.PHONY: all printer client clean

all:
	make client
	make printer

printer: printer.c
	mkdir -pv $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

client: client.c
	mkdir -pv $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

clean:
	rm -rf $(BUILD_DIR)