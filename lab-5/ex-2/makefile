CC = gcc
CFLAGS = -std=c17 -Wall
BUILD_DIR = build

PID = 0
VALUE = 1

.PHONY: all sender catcher send catch clean

all:
	make sender
	make catcher

sender: sender.c
	mkdir -pv $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

catcher: catcher.c
	mkdir -pv $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

send:
	./$(BUILD_DIR)/sender $(PID) $(VALUE)

catch:
	./$(BUILD_DIR)/catcher

clean:
	rm -rf $(BUILD_DIR)