CC = gcc
CFLAGS = -std=c17 -Wall
BUILD_DIR = build

.PHONY: all server client example_server example_client clean

all: server client

server: server.c
	mkdir -pv $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

client: client.c
	mkdir -pv $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

example_server:
	./$(BUILD_DIR)/server 8080

example_client:
	./$(BUILD_DIR)/client pan_klient 0 8080

clean:
	rm -rf $(BUILD_DIR)
