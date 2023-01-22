DD=g++#compiler
CFLAGS=-I.#path to the header files
CFLAGS+=-Wall
FILES=lab1.cpp#source code
LIBS = -L . libPidUtil.a#path to the library and the library itself

Lab1: $(FILES)
	$(DD) $(CFLAGS) $(FILES) -o Lab1 $(LIBS)

clean:
	rm -f *.o Lab1

all: Lab1
