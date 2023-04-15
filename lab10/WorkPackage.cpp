//
//  WorkPackage.cpp - logic of WorkPackage program
//
//  Apr 14, 2023    Created by Yiyuan Dong
//  Student ID: 065-182-131
//

#include "WorkPackage.h"
#include <fcntl.h>
#include <iostream>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

void prompt(WorkPackage *wp_addr, struct stat &sb);

int main(int argc, char const *argv[]) {
    const char workData[] = "WorkData.bin";
    struct stat sb;
    const int MEM_SIZE = sizeof(unsigned int) * 2 + sizeof(double);
    bool fileExisted = true;

    if (stat(workData, &sb) == -1) {
        cout << "File does not exist." << endl;
        fileExisted = false;
    }

    int openFlags = O_RDWR | O_CREAT;
    mode_t filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP; // rw-rw----
    int fd = open(workData, openFlags, filePerms);
    if (fd == -1) {
        cout << "Error opening file: " << strerror(errno) << endl;
        exit(-1);
    }

    if (!fileExisted) {
        char buf[MEM_SIZE];
        memset(buf, 0, MEM_SIZE);
        write(fd, buf, MEM_SIZE);
    }

    WorkPackage *wp_addr;
    // void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
    wp_addr = (WorkPackage *)mmap(NULL, MEM_SIZE, PROT_READ | PROT_WRITE,
                                  MAP_SHARED, fd, 0);
    close(fd); // close when mmap is successful. if failed, have to close it as well
    if (wp_addr == MAP_FAILED) {
        cout << "Error: mmap failed. " << strerror(errno) << endl;
        exit(-3);
    }

    prompt(wp_addr, sb);

    munmap(wp_addr, sb.st_size);

    return 0;
}

void prompt(WorkPackage *wp_addr, struct stat &sb) {
    int hr = 0;
    double amt = 0;
    int selection;
    do {
        system("clear");
        cout << "WORK PACKAGE" << endl
             << "Entries: " << wp_addr->entries << endl
             << "Hours: " << wp_addr->hours << endl
             << "Balance: " << wp_addr->balance << endl
             << endl;
        cout << "Make a selection:" << endl;
        cout << "1.Log Hours." << endl;
        cout << "2.Add Expense." << endl;
        cout << "3.Add Deposit." << endl;
        cout << "4.Refresh." << endl;
        cout << "5.Clear Data." << endl;
        cout << "0.Quit." << endl;
        cout << endl;
        cout << "Selection: ";
        cin >> selection;

    switch (selection) {
    case 1:
        cout<<"Enter the hours: ";
        cin>>hr;
        wp_addr->hours += hr;
        wp_addr->entries += 1;
        break;
    case 2:
        cout<<"Enter the expense amount: ";
        cin>>amt;
        wp_addr->balance -= amt;
        break;
    case 3:
        cout<<"Enter the deposit amount: ";
        cin>>amt;
        wp_addr->balance += amt;
        break;
    case 4:
        break;
    case 5:
        wp_addr->balance = 0;
        wp_addr->entries = 0;
        wp_addr->hours = 0;
        break;
    default:
        break;
    }

    msync(wp_addr, sb.st_size, MS_SYNC);

   } while (selection != 0);
}

/*
Questions

Compare memory mapping with shared memory. What are the similarities and differences?
 Similarities:
    - They both provide fast IPC by allowing processes to exchange data, which is placed in a region of memory that is shared between the processes. 
    and once the shared memory region has been mapped into the process's virtual address space, it looks just like any other part of the process's memory space. And applications typically must use a semaphore or other synchronization primitives to synchronize access to the shared region.

Differences:
    - Memory mapping the data persist across system restart since the content of the shared memory mapping are synchronized with a mapped file. In contrast, shared memory does not persist data across system restart.
    - Memory mapping can be used to read data from a file and write data to a file. This provides a low-overhead IPC machanism when the processes are reading and writing the same file. Yet shared memory is usually used for read and write acess to a block of memory.
    -

When would you use memory mapping instead of shared memory?

    - When the processes needs to map a file or device and accessing the files or devices are frequent
    - When page-level protection is needed
    - When only a small portion of a file needs to be mapped


When would you use shared memory instead of memory mapping?

    - When data is small and randomly/frequently accessed.
    - When shared memory regions need to be shared between unrelated processes
*/