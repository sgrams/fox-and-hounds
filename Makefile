##
# This file is part of fox-and-hounds task.
#
# Stanisław Grams <sgrams@fmdx.pl>
# Makefile
##
CFLAGS=-Wall -Werror -pedantic -std=c99
LDFLAGS=$(pkg-config --libs x11) -lX11

all: build
build: main.o gui.o socket.o board.o
	+$(CC) -o fox *.o $(CFLAGS) $(LDFLAGS)

## objs
main.o: main.c
gui.o: gui.c
socket.o: socket.c
board.o: board.c

## other procedures
.PHONY: clean
rmobj:
	rm -f main.o gui.o socket.o board.o
clean:
	rm -f fox main.o gui.o socket.o board.o
