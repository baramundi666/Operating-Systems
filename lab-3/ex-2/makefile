CC = gcc
CFLAGS = -std=c17 -Wall
BUILD_DIR = build

.PHONY: all  clean

all:
	make dir_size

dir_size: dir_size.c
	mkdir -pv $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

clean:
	rm -rf $(BUILD_DIR)
