CC = gcc
CFLAGS = -std=c17 -Wall -g -O0
BUILD_DIR = build

.PHONY: all collatz_static collatz_shared clean

all:
	make client_static
	make client_shared
	make client_dynamic

client_static: client.c
	mkdir -pv $(BUILD_DIR)
	make -C collatz collatz_static
	$(CC) $(CFLAGS) -c $< -o $(BUILD_DIR)/$@.o
	$(CC) $(CFLAGS) $(BUILD_DIR)/$@.o collatz/build/libcollatz_static.a -o $(BUILD_DIR)/$@

client_shared: client.c
	mkdir -pv $(BUILD_DIR)
	make collatz_shared -C collatz
	$(CC) $(CFLAGS) -L ./ $< collatz/build/libcollatz_shared.so -Wl,-rpath,. -o $(BUILD_DIR)/$@

client_dynamic: client.c
	mkdir -pv $(BUILD_DIR)
	make collatz_shared -C collatz
	$(CC) $(CFLAGS) -rdynamic $< -L ./ -Wl,-rpath,. -o $(BUILD_DIR)/$@ -D DYNAMIC

test:
	./$(BUILD_DIR)/client_static
	./$(BUILD_DIR)/client_shared
	./$(BUILD_DIR)/client_dynamic

clean:
	make -C collatz clean
	rm -rf $(BUILD_DIR)