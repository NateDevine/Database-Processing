CC=gcc
CFLAGS=-ggdb -Wall -fmax-errors=2
LIBS=-lsqlite3

all:	mkpkgdb

mkpkgdb:	mkpkgdb.o setup.o util.o
	$(CC) $(CFLAGS) -o mkpkgdb *.o $(LIBS)

mkpkgdb.o:	mkpkgdb.c mkpkgdb.h

setup.o:	setup.c mkpkgdb.h

util.o:	util.c mkpkgdb.h

clean:
	rm -f *.o

realclean:	clean
	rm -f mkpkgdb
