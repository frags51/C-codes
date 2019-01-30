#ifndef _FTP_AQR_H
#define _FTP_AQR_H value

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/time.h> 
#include <netinet/in.h>
#include <sys/types.h> 
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <cmath>
#include <vector>
#include <sys/time.h>

int checkSum(char *p, int Z) {

    unsigned char checksum = 0x02;

    for(int i=0; i<Z; i++)   {
        checksum ^= *p++;
    }   
    checksum ^= 0x03;
    return (int) checksum;
}
// timeout in microsecs
#define TIMEOUT 100000
int timer = 0;
int toPort;
int thisPort;
using namespace std;

int nextSn=0; // Next free seq number to use!
int expectedSn = 1; // Expected ack number = nextNumber to use (1 ack for 0 seq sent)!

#define MAX_ARGS 40
const int N = 1024; // max number of bytes. 

int getNextSeq(){
    nextSn++;
    return nextSn-1;
}
int setNextExp(){
    expectedSn++;
}

typedef struct _msg{
    int id; // what type of function etc
    int argsize;
    float args[MAX_ARGS];
} msg_struct;

typedef struct _data{
    int csum;
    int seq_no;
    int ack_no;
    bool isAck = false;
    bool isExtra = false;
    bool disconnect = false; // disconnect
    bool start = false; // connect
    int bytes_length = 0;
    char bytes[N];

} Data;

// useless as of now.
typedef enum{frame_arrival,err,timeout,no_event} event_type;


#endif