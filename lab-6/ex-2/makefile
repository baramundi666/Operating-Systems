CC = gcc
CFLAGS = -std=c17 -Wall
BUILD_DIR = build

.PHONY: all user machine user_ machine_ clean

all:
	make user
	make machine

user: user.c
	mkdir -pv $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

machine: machine.c
	mkdir -pv $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

user_:
	./$(BUILD_DIR)/user

machine_:
	./$(BUILD_DIR)/machine

clean:
	rm -rf $(BUILD_DIR)
	rm pipeline