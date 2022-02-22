CC = /usr/bin/gcc
CFLAGS = -Wall -g
LDFLAGS = -lncurses

all: compile run

compile:
	$(CC) $(CFLAGS) $(LDFLAGS) ./src/core/main.c ./src/module/board.c ./src/module/engine.c ./src/module/list.c ./src/module/tree.c -o main

run: compile
	./main

clean:
	rm -f ./main
