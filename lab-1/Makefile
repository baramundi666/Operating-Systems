CC = gcc
CFLAGS = -Wall -std=c17

.PHONY : all countdown clean

all:
	make countdown

countdown: countdown.c
	@echo "countdown compilation"
	$(CC) $(CFLAGS) -o $@ $< -g

clean :
	@echo "clean compilation files"
	rm countdown



