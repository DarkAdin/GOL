CC = gcc

all:
	$(CC) gol.c -o GOL -Wall -Wextra -march=native
