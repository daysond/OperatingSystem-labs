//
//  server.cpp - SEP 400 Lab 6
//  Created by:  Yiyuan Dong    2023-03-14
//

#include "client.h"
#include <errno.h>
#include <iostream>
#include <pthread.h>
#include <queue>
#include <signal.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define MTYPE = 4

using namespace std;
//  Function Signatures
void signalHandler(int);
void *recv_func(void *arg);
void *wr_func(void *arg);

void setupSignalHandler();
void setupMsgQueue();
void startThreads(pthread_t*, pthread_t*);

//  Global Variables
const int NUM_CL = 3;       //  number of clients
pthread_mutex_t lock_x;
queue<Message> message;
bool is_running;
key_t key;
int msgid, ret;

int main(int argc, char const *argv[]) {

    pthread_t recv_tid;
    pthread_t wr_tid;
   
   //   start up
    is_running = true;
    pthread_mutex_init(&lock_x, NULL);
    setupSignalHandler();
    setupMsgQueue();
    startThreads(&recv_tid, &wr_tid);
    cout << "[Server] Running...\n Press Ctrl + C to quit." << endl;
    //  clean up
    pthread_join(recv_tid, NULL);
    pthread_join(wr_tid, NULL);
    msgctl(msgid, IPC_RMID, NULL);
    cout << "[Server] Quitting..." << endl;

    return 0;
}

void *recv_func(void *arg) {
    cout << "[Thread] Receive thread started." << endl;
    while (is_running) {
        Message msg;
        ret = msgrcv(msgid, &msg, sizeof(msg), MTYPE,
                     IPC_NOWAIT); 
        if (ret >= 0) {
            pthread_mutex_lock(&lock_x);
            message.push(msg);
            pthread_mutex_unlock(&lock_x);
        } else {
            sleep(1);
        }
    }

    pthread_exit(NULL);
}

void *wr_func(void *arg) {
    cout << "[Thread] Write thread started." << endl;
    while (is_running) {
        while (message.size() > 0) {
            pthread_mutex_lock(&lock_x);
            Message msg = message.front();
            message.pop();
            pthread_mutex_unlock(&lock_x);
            msg.mtype = msg.msgBuf.dest;
            msgsnd(msgid, &msg, sizeof(msg), 0);
        }
    }
    // send quit to clients
    Message msg;
    sprintf(msg.msgBuf.buf, "Quit");
    for (int i = 0; i < NUM_CL; i++) {
        msg.mtype = i + 1;
        msgsnd(msgid, &msg, sizeof(msg), 0);
    }
    pthread_exit(NULL);
}

void signalHandler(int signum) {
    switch (signum) {
    case SIGINT:
        cout << "\n[Interrupt] SIGINT received." << endl;
        is_running = false;
        break;

    default:
        cout << "Handler not defined." << endl;
        break;
    }
}

void setupSignalHandler() {
    struct sigaction action;
    action.sa_handler = signalHandler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = SA_RESTART;
    sigaction(SIGINT, &action, NULL);
}

void setupMsgQueue() {
    key = ftok("serverclient", 65);
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid < 0) {
        cout << "[ERROR] " << strerror(errno) << endl;
        exit(-1);
    }
}

void startThreads(pthread_t *recv_tid, pthread_t *wr_tid) {
    ret = pthread_create(recv_tid, NULL, recv_func, NULL);
    if (ret != 0) {
        is_running = false;
        cout << strerror(errno) << endl;
        exit(-1);
    }

    ret = pthread_create(wr_tid, NULL, wr_func, NULL);
    if (ret != 0) {
        is_running = false;
        cout << strerror(errno) << endl;
        exit(-1);
    }
}