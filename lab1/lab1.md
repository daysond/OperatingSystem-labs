
# SEP400 - Lab 1: Using a Static Library

## Due: Friday, January 27, 2023

In this lab you will link a static library into your build and use the available functions inside it. You are not to create this static library. You are to use this static library along with its header file. 

The static library is [libPidUtil.a](libPidUtil.a) and its header file is [pidUtil.h](pidUtil.h). Take a look at the functions in **pidUtil.h** because your code will be calling these functions. You can also look inside the contents of **libPidUtil.a** using nm as follows:

```
nm libPidUtil.a | grep ' T '
```

## PART A

---
Your job is to create a **Makefile** and **Lab1.cpp** which links this static library in the build process and uses some of the functions inside. **Lab1.cpp** must:

1. Call **GetAllPids()** and **GetNameByPid()** to print out all pids and their names.
2. Set pid to 1. Call **GetNameByPid()** and print out the name of pid 1.
3. Set name to "Lab1". Call **GetPidByName()** to get the pid of Lab1. Print "Lab1" and the pid of Lab1.
4. Set name to "Lab11". Call **GetPidByName()** to get the pid of Lab11. There should not be a process called Lab11, therefore this should test your error message generation system.
5. If any errors are generated in the calls to these functions, the error must be printed out by a call to the function **GetErrorMsg(**) with the error number as an argument.

## PART B

---
The Makefile to create **libPidUtil.a** is as follows:

```cpp
CC=g++
CFLAGS=-I
CFLAGS+=-Wall
CFLAGS+=-c
AR=ar
pidUtil: pidUtil.cpp
    $(CC) $(CFLAGS) pidUtil.cpp -o pidUtil.o

lib: pidUtil.o
    $(AR) rcs libPidUtil.a pidUtil.o

clean:
    rm -f *.o *.a

install:
    cp libPidUtil.a ../.
    cp pidUtil.h ../.

all: pidUtil lib
```

In a Word document, please explain every line of this Makefile.
