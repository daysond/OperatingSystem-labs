
# Lab 1 - Part 2: Makefile explained

## Define g++ compiler variable. 
```C
CC=g++
```
## Define Flags:

- Indicates the path to header files, empty or dot means current folder
```C
CFLAGS=-I
```
```C++
CFLAGS+=-Wall
```
```C
CFLAGS+=-c
```
```C
AR=ar
```
```C
pidUtil: pidUtil.cpp
    $(CC) $(CFLAGS) pidUtil.cpp -o pidUtil.o
```
```C
lib: pidUtil.o
    $(AR) rcs libPidUtil.a pidUtil.o
```
```C
clean:
    rm -f *.o *.a
```
```C
install:
    cp libPidUtil.a ../.
    cp pidUtil.h ../.
```

```C
all: pidUtil lib
```