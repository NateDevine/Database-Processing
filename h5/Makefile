CC=gcc
CFLAGS=-ggdb -Wall
LIBS=-lsqlite3

all: mkpkgdb

mkpkgdb:	mkpkgdb.c
	gcc -ggdb -Wall -o mkpkgdb mkpkgdb.c -lsqlite3

clean:
	rm -f *.o

realclean:	clean
	rm -f mkpkgdb
