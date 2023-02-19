//
//  server.cpp - SEP 400 Lab 4
//  Created by:  Yiyuan Dong    2023-02-18
//

#include <arpa/inet.h>
#include <iostream>
#include <net/if.h>
#include <netinet/in.h>
#include <queue>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

using namespace std;

#define DEBUG

const char LOCALHOST[] = "127.0.0.1";
bool isRunning = true;
const int BUF_LEN = 4096;
const int NUMCLIENT = 3;
typedef struct message {
    int len;
    char buf[BUF_LEN];
} Message;

queue<Message> messageQueue;
pthread_mutex_t lock_x;

int setupAddr(struct sockaddr_in &addr, int &sock, const char *port);
void errExit(string, int &);

int main(int argc, char const *argv[]) {

    //  no port number provided, return -1
    if (argc < 2) {
        cout << "usage: client <port number>" << endl;
        return -1;
    }

    int ret;
    int connections = 0;
    int clients[NUMCLIENT];
    sockaddr_in cl_addrs[NUMCLIENT];
    char buf[BUF_LEN];
    struct sockaddr_in addr;

    //  create a socket
    int master_socket = socket(AF_INET, SOCK_STREAM, 0);
    int flags = fcntl(master_socket, F_GETFL, 0);
    fcntl(master_socket, F_SETFL, flags | O_NONBLOCK);

    //  setup addr (hint)
    ret = setupAddr(addr, master_socket, argv[1]);
    if (ret == 0)
        errExit("No such address", master_socket);

    //  bind socket to addr
    if ((bind(master_socket, (struct sockaddr *)&addr, sizeof(addr))) == -1)
        errExit("Cannot bind the socket to the local address", master_socket);

    //  mark the socket for listening in
    if (listen(master_socket, 5) == -1)
        errExit("Server: cannot listen", master_socket);

    while (isRunning) {
        while (connections < 3)
        {   
            #if defined(DEBUG)
            cout << "accept()" << endl;
            #endif

            socklen_t cl_size = sizeof(cl_addrs[connections]);
            if ( (clients[connections] = accept(master_socket, (sockaddr* ) &cl_addrs[connections],  &cl_size)) == -1) 
                errExit("Server: cannot accept client", master_socket);
            else
                connections += 1;

            #if defined(DEBUG)
            cout << "accepting..." << endl;
            #endif
        }
        

    }

    return 0;
}

int setupAddr(struct sockaddr_in &addr, int &sock, const char *port) {
    memset((char *)&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port)); //  host to network short
    return inet_pton(
        AF_INET, LOCALHOST,
        &addr.sin_addr); //  inet_pton: internet command, convert pointer to a
                         //  string to a number, 0.0.0.0 to get any address
}

void errExit(string err, int &sock) {
    cout << err << endl;
    cout << strerror(errno) << endl;
    close(sock);
    exit(-1);
}
