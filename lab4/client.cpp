//
//  client.cpp
//  Created by:  Yiyuan Dong    2023-02-15
//
#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;
#define DEBUG
char socket_path[] = "/tmp/lab4";

int main(int argc, char const *argv[])
{   
    int sock, server, bytes, size;
    char buf[256];
    bool isRunning = true;
    struct sockaddr_un addr;

    memset(&addr, 0, sizeof(addr));
    //  set family
    addr.sun_family = AF_UNIX;
    //  set socket path to the addr info
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);

    #if defined(DEBUG)
    cout << "client: addr.sun_path:" << addr.sun_path << endl;
    #endif

        //  Create the socket
    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0) {
        cerr << "server: " << strerror(errno) << endl;
        exit(-1);
    }

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        cerr << "client connect() failed.: " << strerror(errno) << endl;
        close(sock);
        exit(-1);
    }

    #if defined(DEBUG)
    cout << "client: connect()" << endl;
    #endif

    while (isRunning) {
        #if defined(DEBUG)
        cout << "client: running()" << endl;
        #endif
        
        memset(&buf, 0, sizeof(buf));
        while ((bytes = read(sock, buf, 255)) > 0) {
            
            cout << "Reveived request from server: " << buf << endl;

            if (bytes < 0) {
                perror("ERROR reading from socket");
                exit(1);
            }

            // send back pid
            if(strncmp("pid", buf, 3)==0) {
                memset(&buf, 0, sizeof(buf));
                size = 26;
                snprintf(buf, size ,"This client has pid %d" ,getpid());
                bytes = write(sock, buf, size);
                if (bytes != size) {
                    if (bytes > 0)
                        fprintf(stderr, "partial write");
                    else {
                        cout << "client: " << strerror(errno) << endl;
                        close(sock);
                        exit(-1);
                    }
                }
            }

            if(strncmp("Sleep", buf, 5) == 0) {
                cout<< "The client is going to sleep for 5 seconds." << endl;
                sleep(5);
                memset(&buf, 0, sizeof(buf));
                size = 4;
                strncpy(buf, "Done", bytes);
                bytes = write(sock, buf, size);
                if (bytes != size) {
                    if (bytes > 0)
                        fprintf(stderr, "partial write");
                    else {
                        cout << "client: " << strerror(errno) << endl;
                        close(sock);
                        exit(-1);
                    }
                }
            }

            if(strncmp("Quit", buf, 5) == 0) {
                #if defined(DEBUG)
                cout << "The client is quitting." << endl;
                #endif
                bytes = -1;
                isRunning = false;
            }

            memset(&buf, 0, sizeof(buf));
        }
    }
    

    cout << "client: closing socket..." << endl;
    close(sock);
    return 0;
}
