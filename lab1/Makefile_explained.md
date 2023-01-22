
# Lab 1 - Part 2: Makefile explained

## Define Compiler Variable CC 
<br />

- Using g++ compiler
```C
CC=g++
```
<br />

## Define Flags Variable CFLAGS
<br />

- **_'-I '_** Indicates the path to header files, following by nothing or a dot means current folder
```cpp
CFLAGS=-I
```
<br />

- Appends another option to **_CFLAGS_**, **_' -Wall_ '** enables all the compiler's warning messages
```cpp
CFLAGS+=-Wall
```
<br />

- Appends another option to **_CFLAGS_**, **_' -c_ '** tells the compiler to compile to object code only and do not link.
```cpp
CFLAGS+=-c
```
<br />

## Define Archiver Variable AR
- Using ar to archive files
```cpp
AR=ar
```
<br />

## Define Build Rules
<br />

- Target: **_pidUtil_**, Dependency: **pidUtil.cpp**
- Compiles **pidUtil.cpp** using **_g++_** compiler, outputs an object file **pidUtil.o** with options specified in **_CFLAGS_**
```cpp
pidUtil: pidUtil.cpp
    $(CC) $(CFLAGS) pidUtil.cpp -o pidUtil.o
```
<br />

- Target: **_lib_**, Dependency: **pidUtil.o**
- Creates an archive **'libPidUtil.a'** insert the file **'pidUtil.o'** into the archive with options **'rcs'**
    - Option **'r'**: insert the file members into the archive with replacement.
    - Option **'c'**: create the archive.
    - Option **'s'**: write an object-file index into the archive or update an existing one.
```cpp
lib: pidUtil.o
    $(AR) rcs libPidUtil.a pidUtil.o
```
<br />

- Target: **_clean_**
- Removes all the files with suffixes **'.o'** and **'.a'** generated during the build process
```cpp
clean:
    rm -f *.o *.a
```
<br />

- Target: **_install_**
- Copies **'libPidUtil.a'** and **'pidUtil.h'** and paste it into the parent folder
```cpp
install:
    cp libPidUtil.a ../.
    cp pidUtil.h ../.
```
<br />

- Target: **all**
- Runs target **pidUtil** and **lib**
```cpp
all: pidUtil lib
```