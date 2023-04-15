CC=g++
CFLAGS=-I
CFLAGS+=-Wall
FILES=WorkPackage.cpp

workPackage: $(FILES)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f *.o workPackage

all: workPackage
