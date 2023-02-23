//
//  server.cpp - SEP 400 Lab 4
//  Created by:  Yiyuan Dong    2023-02-18
//

#include <errno.h>
#include <arpa/inet.h>
#include <fcntl.h> //  for nonblocking
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
// typedef struct message {
//     int len;
//     char buf[BUF_LEN];
// } Message;

queue<string> messageQueue;
pthread_mutex_t lock_x;

int setupAddr(struct sockaddr_in &addr, int &sock, const char *port);
void errExit(string, int &);
void *recv_func(void *arg);

int main(int argc, char const *argv[]) {

    //  no port number provided, return -1
    if (argc < 2) {
        cout << "usage: client <port number>" << endl;
        return -1;
    }

    int ret;
    int connections = 0;
    int clients[NUMCLIENT];
    char buf[BUF_LEN];
    sockaddr_in cl_addrs[NUMCLIENT];
    struct sockaddr_in addr;
    pthread_t tids[NUMCLIENT];

    memset(&clients, 0, sizeof(clients));

    cout << clients[0] << clients[1] << clients[2] << endl;
    //  create a socket
    int master_socket = socket(AF_INET, SOCK_STREAM, 0);

    // set socket timeout / non-blocking
    // struct timeval tv;
    // tv.tv_sec = 5;
    // tv.tv_usec = 0;
    // setsockopt(master_socket, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv,
    //            sizeof(tv));

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

    pthread_mutex_init(&lock_x, NULL);

    while (isRunning) {
        if (connections < 3) {
            socklen_t cl_size = sizeof(cl_addrs[connections]);
            ret = clients[connections] = accept(
                master_socket, (sockaddr *)&cl_addrs[connections], &cl_size);
#if defined(DEBUG)
            cout << "accept()ed client " << clients[connections] << endl;
#endif
            if (ret == -1) {
                errExit("Server: cannot accept client", master_socket);
            } else {
// once connect , start recv thread
// create thread
#if defined(DEBUG)
                cout << "creating thread () client: " << clients[connections]
                     << endl;
#endif
                ret = pthread_create(&tids[connections], NULL, recv_func,
                                     &clients[connections]);
                if (ret != 0)
                    errExit("Cannot create receive thread", master_socket);
                connections += 1;
            }
        }

        if (messageQueue.empty() == 0) {
            pthread_mutex_lock(&lock_x);
            cout << messageQueue.front() << endl;
            messageQueue.pop();
            pthread_mutex_unlock(&lock_x);
        }

        sleep(1);
    }

    //  join threads here

    // clean up...

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

void *recv_func(void *arg) {
    int client_fd = *(int *)arg;
    char buf[BUF_LEN];

#if defined(DEBUG)
    cout << "read() client: " << client_fd << endl;
#endif

    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    setsockopt(client_fd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv,
               sizeof(tv));

    while (isRunning) {
        cout << "trying to read... from " << client_fd << endl;
        if (read(client_fd, buf, BUF_LEN) > 0) {
            pthread_mutex_lock(&lock_x);
            messageQueue.push(buf);
            pthread_mutex_unlock(&lock_x);
        } else {
            cout << strerror(errno) << endl;
        }
    }

    pthread_exit(NULL);
}