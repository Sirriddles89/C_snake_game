#include <ncurses.h>

all: main

main: snake.c
	cc -o snake snake.c -lncurses