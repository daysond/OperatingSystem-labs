//
//  server.cpp
//  Created by:  Yiyuan Dong    2023-02-14
//

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>

using namespace std;
#define DEBUG
//  Define sokcet path
char socket_path[] = "/tmp/lab4";

int main(int argc, char const *argv[]) {

    int sock, client, bytes, size;
    char buf[256];
    bool isRunning = true;
    struct sockaddr_un addr;

    //  Create the socket
    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0) {
        cerr << "server: " << strerror(errno) << endl;
        exit(-1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);

    //  unlink in case socket already exists
    unlink(socket_path);

    #if defined(DEBUG)
    cout << "server: addr.sun_path:" << addr.sun_path << endl;
    #endif
    
    //  Bind the socket to IP / port
    #if defined(DEBUG)
    cout << "server: bind()" << endl;
    #endif
    if(bind(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        cout << "server: " << strerror(errno) << endl;
        close(sock);
        exit(-1);
    }
    //  Mark the socket for listening in
    #if defined(DEBUG)
    cout << "server: listen()" << endl;
    #endif

    if(listen(sock, 5) == -1) {
        cout << "server: " << strerror(errno) << endl;
        unlink(socket_path);
        close(sock);
        exit(-1);
    }
    //  Accept a call
    while (isRunning) {

        if ( (client = accept(sock, NULL, NULL)) == -1) {
            cout << "server: " << strerror(errno) << endl;
            unlink(socket_path);
            close(sock);
            exit(-1);
        }
        #if defined(DEBUG)
        cout << "server: accept() client: " << client << endl;
        #endif

        // send pid request
        bytes = write(client, "pid", 3);
        if(bytes < 0) {
            cout << "Server: " << strerror(errno) << endl;
            unlink(socket_path);
            close(sock);
            exit(-1);
        } 
        cout << "The server requests the client's pid" << endl;
        // reading pid
        memset(&buf, 0, sizeof(buf));
        bytes = read(client, buf, 255);
        if (bytes < 0) {
            perror("ERROR reading from socket");
            exit(1);
        }
        cout << "Server: "<< buf << endl;

        // seding Sleep
        cout << "The server requests the client to sleep" << endl;
        memset(&buf, 0, sizeof(buf));
        size = 5;
        bytes = write(client, "Sleep", size);
        if (bytes != size) {
            if (bytes > 0)
                fprintf(stderr, "partial write");
            else {
                cout << "client: " << strerror(errno) << endl;
                close(sock);
                exit(-1);
            }
        }
        // reading Done
        memset(&buf, 0, sizeof(buf));
        bytes = read(client, buf, 255);
        if (bytes < 0) {
            perror("ERROR reading from socket");
            exit(1);
        }

        // sending Quit
        cout << "The server requests the client to quit" << endl;
        memset(&buf, 0, sizeof(buf));
        bytes = write(client, "Quit", size);
        if (bytes != size) {
            if (bytes > 0)
                fprintf(stderr, "partial write");
            else {
                cout << "client: " << strerror(errno) << endl;
                close(sock);
                exit(-1);
            }
        }
        isRunning = false;

    }

    cout << "server: closing sockets..." << endl;
    unlink(socket_path);
    close(sock);
    close(client);
    return 0;
}

