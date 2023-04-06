// client2.cpp - A client that communicates with a second client using triple RSA encrpytion/decryption
#include <arpa/inet.h>
#include <iostream>
#include <math.h>
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

const char IP_ADDR[]="127.0.0.1";
const int BUF_LEN=256;
bool is_running;
int srcPort=1155;
int destPort=1153;
// Encrpytion/Decryption variables
double n;
double e;
double d;
double phi;

queue<unsigned char*> messageQueue;

pthread_mutex_t lock_x;

int src_fd;
sockaddr_in src_addr, dest_addr;
pthread_t recv_tid, send_tid;
void check(int);
void *recv_func(void *arg);
void *send_func(void *arg);
void decryptMsg(unsigned int *enc_text);
void encryptMsg(const unsigned char (&src)[BUF_LEN], unsigned int (&dest)[BUF_LEN]);

static void shutdownHandler(int sig)
{
    switch(sig) {
        case SIGINT:
            is_running=false;
            break;
    }
}

// Returns a^b mod c
unsigned int PowerMod(int a, int b, int c)
{
    int res = 1;
    for(int i=0; i<b; ++i) {
        res=fmod(res*a, c);
    }
    return (unsigned int)res;
}
  
// Returns gcd of a and b
int gcd(int a, int h)
{
    int temp;
    while (1)
    {
        temp = a%h;
        if (temp == 0)
          return h;
        a = h;
        h = temp;
    }
}
  
// Code to demonstrate RSA algorithm
int main()
{
    // Two random prime numbers
    double p = 11;
    double q = 23;
  
    // First part of public key:
    n = p*q;
  
    // Finding other part of public key.
    // e stands for encrypt
    e = 2;
    phi = (p-1)*(q-1);
    while (e < phi)
    {
        // e must be co-prime to phi and
        // smaller than phi.
        if (gcd((int)e, (int)phi)==1)
            break;
        else
            e++;
  
    }
    // Private key (d stands for decrypt)
    // choosing d such that it satisfies
    // d*e = 1 + k * totient
    int k = 2;  // A constant value
    d = (1 + (k*phi))/e;
    cout<<"p:"<<p<<" q:"<<q<<" n:"<<n<<" phi:"<<phi<<" e:"<<e<<" d:"<<d<<endl;

    signal(SIGINT, shutdownHandler);
    pthread_mutex_init(&lock_x, NULL);
    //TODO: Complete the rest
    // --------- socket set up ---------
    src_fd = socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK, 0);
    check(src_fd);
    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.sin_family = AF_INET;
    src_addr.sin_port = htons(srcPort);
    check(inet_pton(AF_INET, IP_ADDR, &src_addr.sin_addr));

    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(destPort);
    check(inet_pton(AF_INET, IP_ADDR, &dest_addr.sin_addr));

    check(bind(src_fd, (struct sockaddr*)&src_addr, sizeof(src_addr)));
    
    //  --------- Starting Threads ---------
    is_running = true;
    check(pthread_create(&recv_tid, NULL, recv_func, &src_fd));
    sleep(5);
    check(pthread_create(&send_tid, NULL, send_func, &src_fd));  

    // --------- Reading msg ---------
    unsigned char *dec_msg = nullptr;
    while(is_running) {
        pthread_mutex_lock(&lock_x);
        if (messageQueue.empty() == 0) {
            dec_msg = messageQueue.front();
            messageQueue.front() = nullptr;
            messageQueue.pop();
        }
        pthread_mutex_unlock(&lock_x);
        
        if(dec_msg) {
            if(strncmp((char *)dec_msg, "Quit", 4)==0) is_running=false;
            else cout << "Received: " << dec_msg << endl;
            delete [] dec_msg;
            dec_msg = nullptr;
        }
        sleep(1);
    }

    // --------- Exiting ---------
    pthread_join(send_tid, NULL);
    pthread_join(recv_tid, NULL);
    close(src_fd);

    cout << "client2 is quitting..." << endl;

    return 0;

}

//TODO: Complete the receive thread

void *recv_func(void *arg) {

    int src_fd = *(int *)arg;
    int msg_len = 0;
    unsigned int enc_text[BUF_LEN];
    unsigned char dec_text[BUF_LEN];
    memset(&enc_text, 0, BUF_LEN);
    memset(&dec_text, 0, BUF_LEN);
    while(is_running) {
        msg_len = recvfrom(src_fd, enc_text, BUF_LEN * sizeof(unsigned int), 0, NULL, NULL);
        if(msg_len < 0) {
            sleep(1);
        } else {
            decryptMsg(enc_text);
            memset(&enc_text, 0, BUF_LEN);
        }
    }
    pthread_exit(NULL);
}

void decryptMsg(unsigned int *enc_text) {
    // This function takes a double array, decrypts the array into a string and pushes it to message queue
    unsigned char* dec_text = new unsigned char[BUF_LEN];
    memset(dec_text, 0, BUF_LEN);
    int c = 0;
    int i = 0;
    do {
        c = int((*enc_text));
        dec_text[i] = (unsigned char)PowerMod(c, d, n); // m= c^ d mod(n)
        i++;
        enc_text++; 

    } while(c != 0);
 
    pthread_mutex_lock(&lock_x);
    messageQueue.push(dec_text);
    pthread_mutex_unlock(&lock_x);

}

void *send_func(void *arg) {

    int src_fd = *(int *)arg;

    const int numMessages=5;
    const unsigned char messages[numMessages][BUF_LEN]={
            "You were lucky to have a room. We used to have to live in a corridor.",
            "Oh we used to dream of livin' in a corridor! Woulda' been a palace to us.",
            "We used to live in an old water tank on a rubbish tip.",
            "We got woken up every morning by having a load of rotting fish dumped all over us.",
            "Quit"};

    unsigned int cipherText[numMessages][BUF_LEN];

    // Encrypting msg
    for(int i = 0; i < numMessages; i++) {
        memset(&cipherText[i], 0, sizeof(cipherText[i]));
        encryptMsg(messages[i], cipherText[i]);
    }

    for (int i = 0; i < numMessages; i++) {
        sendto(src_fd, (const unsigned int*)cipherText[i] , BUF_LEN * sizeof(unsigned int), 0,(struct sockaddr *)&dest_addr, sizeof(dest_addr));
        sleep(1);
    }

    pthread_exit(NULL);
}

void encryptMsg(const unsigned char (&src)[BUF_LEN], unsigned int (&dest)[BUF_LEN]) {
    // This function takes a string as src and a double array as dest, encrypt every char into double
    int m = 0;
    int i = 0;
        do {
            m = int(src[i]);
            unsigned int res = PowerMod(m, e, n);  // c = m ^ e mod (n)
            dest[i] = res;
            i++;
        } while(m != 0);
}



void check(int ret){
    /*
     This function checks for error. It takes a returned status and a socket fd.
     If there's an error, it closes the socket and exits with returned status.
    */
    if (ret < 0){
        if(errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) return;
        else{
            cerr << strerror(errno) << endl;
            close(src_fd); 
            exit(ret);
        }    
    }
}
