// hack.cpp - A program that attempts to break the RSA encryption while varying the
//            priority of the current process
//
//
#include <fstream>
#include <iostream>
#include <math.h>
#include <sched.h>
#include <sys/resource.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

bool DeterminePrimes();

// Code to hack the RSA algorithm
int main()
{
    int ret=0;
    int retVal=0;
    const long nanosecsPerSecond=1000000000;
    timespec startTime, endTime;
    double totalTime;

    const char filename[] = "./time_data.txt";
    //TODO: Open a file
    int fd = open(filename, O_WRONLY | O_CREAT , 0777);
  
    if (fd == -1) {
        cout << "Error: " << strerror(errno) << endl;
    }


    //TODO: Call DeterminePrimes() 20 times while varying the
    //      priority from -20 to +19
    int nice_value;
    const int BUF_LEN = 256;
    char buf[BUF_LEN];
    for (int i=0; i<40; i++) {
        nice_value = -20+i;

        retVal = setpriority(PRIO_PROCESS, 0, nice_value);
        nice_value = getpriority(PRIO_PROCESS, 0);//validate the set

        if(retVal < 0) {
            cout<< "[Error]" << strerror(errno) <<": did you forget \'sudo\' ? \n exiting..."<< endl;
            exit(retVal);
        }

        cout<<"retVal:"<<retVal<<" niceValue:"<<nice_value<<endl;
        
        //timer start
        clock_gettime(CLOCK_REALTIME, &startTime);
        DeterminePrimes();
        //timer stop
        clock_gettime(CLOCK_REALTIME, &endTime);
        // total time in nano seconds
        totalTime = nanosecsPerSecond * (endTime.tv_sec - startTime.tv_sec) + endTime.tv_nsec - startTime.tv_nsec;
        
        cout<< nice_value <<": " << totalTime << endl;
        memset(&buf, 0, BUF_LEN);
        ret =sprintf(buf, "%d:%lf", nice_value,totalTime)+1;
        buf[ret-1] = '\n';
        write(fd, buf, ret);
    }

    //TODO: Be sure to close your file when done
    close(fd);
    return ret;
}

bool DeterminePrimes() {
    unsigned long long n=5335952767283123;//49182449ull * 108493027ull
    unsigned long long p=2;
    unsigned long long q=0;
    long double temp;
    bool found=false;
    while(!found && p<n) {
        temp = (long double)n/p;
        q = n/p;
	    if(temp==(long double)q) found=true;//no numbers after the decimal place
                                            //means we have found a factor
	    else ++p;//Usually we would call a function to get the next prime number
	             //instead of simply incrementing by one. But such a function does
                 //basically the same thing as we are doing here.
    }
    if(found) {
        cout<<"p:"<<p<<" q:"<<q<<" n:"<<n<<endl;
    } else {
        cout<<"factors not found for n:"<<n<<endl;
    }
    return found;
}
