CC = gcc
CFLAGS = -std=c17 -Wall
BUILD_DIR = build

.PHONY: all server client create_server create_client clean

all:
	make client
	make server

server: server.c
	mkdir -pv $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

client: client.c
	mkdir -pv $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

create_server:
	./build/server

create_client:
	./build/client

clean:
	rm -rf $(BUILD_DIR)