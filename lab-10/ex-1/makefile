CC = gcc
CFLAGS = -std=c17 -Wall
BUILD_DIR = build

.PHONY: all santa run_santa clean

all:
	make santa

santa: santa.c
	mkdir -pv $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

run_santa:
	./build/santa

clean:
	rm -rf $(BUILD_DIR)