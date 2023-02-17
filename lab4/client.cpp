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

char socket_path[] = "/tmp/lab4";

int main(int argc, char const *argv[])
{   
    int sock, bytesRecv;
    char buf[1024];
    bool isRunning = true;

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    //  set family
    addr.sun_family = AF_UNIX;
    //  set socket path to the addr info
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);
    //  unlink in case socket already exists
    unlink(socket_path);

    #if defined(DEBUG)
    cout << "client: addr.sun_path:" << addr.sun_path << endl;
    #endif

        //  Create the socket
    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0) {
        cerr << "server: " << strerror(errno) << endl;
        exit(-1);
    }

    #if defined(DEBUG)
    cout << "client: connect()" << endl;
    #endif

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        cerr << "client: " << strerror(errno) << endl;
        close(sock);
        exit(-1);
    }

    while (isRunning) {
        bytesRecv = read(STDIN_FILENO, buf, sizeof(buf));
        cout << "client1: write(" << buf << ")" << endl;
        // Write to the socket
        if (write(sock, buf, bytesRecv) != bytesRecv) {
            if (bytesRecv > 0)
                fprintf(stderr, "partial write");
            else {
                cout << "client1: " << strerror(errno) << endl;
                close(sock);
                exit(-1);
            }
        }
        if (strncmp("quit", buf, 4) == 0) {
            isRunning = false;
        }
    }
    

    cout << "client: close()" << endl;
    close(sock);
    return 0;
}
