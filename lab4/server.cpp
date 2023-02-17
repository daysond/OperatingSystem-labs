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

//  Define sokcet path
char socket_path[] = "/tmp/lab4";
int main(int argc, char const *argv[])
{
    int sock, client, bytesRecv;
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
    cout << "server: addr.sun_path:" << addr.sun_path << endl;
    #endif
    
    //  Create the socket
    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0) {
        cerr << "server: " << strerror(errno) << endl;
        exit(-1);
    }
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
    while (isRunning)
    {
        #if defined(DEBUG)
        cout << "server: accept()" << endl;
        #endif
        if ( (client = accept(sock, NULL, NULL)) == -1) {
            cout << "server: " << strerror(errno) << endl;
            unlink(socket_path);
            //  Close the listening socket
            close(sock);
            exit(-1);
        }

        //The server will send the command "Pid" to the client to request the client's pid.
        if (write(client, "Pid", 3) != 3) {
            if (bytesRecv > 0)
                fprintf(stderr, "partial write");
            else {
                cout << "client1: " << strerror(errno) << endl;
                close(sock);
                exit(-1);
            }
        }
        

        #if defined(DEBUG)
        cout << "server: read()" << endl;
        #endif
        //  while receiving message, display / response
        while (bytesRecv == read(client, buf, sizeof(buf)) > 0) {
            cout << "read " << bytesRecv << " bytes: " << buf << endl;
            if(strncmp("quit", buf, 4)==0) {
	            isRunning = false;
            }

            if(bytesRecv < 0) {
                cout << "server: " << strerror(errno) << endl;
                unlink(socket_path);
                close(sock);
                close(client);
                exit(-1);
            }
            if (bytesRecv == 0) {
                //  end of file
                unlink(socket_path);
                close(sock);
                close(client);
            }
        }  
    }
//  Close socket
    cout << "server: close(fd), close(cl)" << endl;
    unlink(socket_path);
    close(sock);
    close(client);
    return 0;

    
    
    
}
