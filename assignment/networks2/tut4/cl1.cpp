#include <iostream>
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

#define TIMEOUT 20
int timer = 0;
int toPort;
int thisPort;
using namespace std;

#define MAX_ARGS 40
const int N = 4; // max number of bytes. 

bool STARTCONN = true; 

int mySocket;
sockaddr_in toAddr;

int nextSn=0;
int expectedSn = 1;
bool sendFlag = true;

bool senderTurn = true;

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
    int seq_no;
    int ack_no;
    bool isAck = false;
    bool isExtra = false;
    bool fl1 = false; // disconnect
    bool fl2 = false; // connect
    char bytes[N];

} Data;

typedef enum{frame_arrival,err,timeout,no_event} event_type;

Data object;

void senderFun(char *);
void recvFun();

char *getData(){
    static int b = 0;
    if (b<5){
        char *c = (char *) malloc(4*sizeof(char));
        strcpy(c, "hel");
        b++;
        return c;
    }
    else return NULL;
}

void pollEventSender(event_type *ev){
    if(!senderTurn){ // receiver!
        *ev = frame_arrival;
    }
    else{ // sender!

    }

}

int doSend(){
    sockaddr_in toAddr;
    toAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    toAddr.sin_port = htons(toPort);
    toAddr.sin_family = AF_INET;

    size_t echoStringLen = sizeof(Data);
    if(sendto(mySocket, &object, echoStringLen, 0, (sockaddr * )&toAddr, sizeof(toAddr))<0){
        cerr<<"Error in sending"<<endl;
        return -1;
    }
    return 0;
}
int doRecv(Data *rBuf){
    if(recvfrom(mySocket, rBuf, sizeof(Data), 0, NULL, NULL) < 0) {
        cerr<<"Error in recv"<<endl;   
        return -1;
    }
    return 0;
}

int main(int argc, char **argv){
    thisPort = atoi(argv[1]);
    toPort = atoi(argv[2]);

    mySocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(mySocket < 0) std::cerr<<"Error, Socket Creation failed!"<<std::endl;

    sockaddr_in servAddr;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_family = AF_INET;
    servAddr.sin_port= htons(thisPort);

    if (bind(mySocket, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
        cerr<<"Error, bind failed!"<<endl;

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 100000;
    if (setsockopt(mySocket, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
        perror("Error");
    }

    toAddr;
    toAddr.sin_family = AF_INET;
    toAddr.sin_port = htons(toPort);
    toAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    while(true){
        char* c;
        if(!sendFlag || (c=(getData())) !=NULL){
            senderFun(c);
            recvFun();
        }
        else break;   
    }
    object.fl1 = true;
    doSend();
    close(mySocket);
} //main

void senderFun(char sbytes[N]){

    event_type event;

    /*if(STARTCONN){
        object.fl2 = true;
        object.seq_no = getNextSeq();
        
        cout<<"Starting conn. "<<endl;
        doSend();
        senderTurn=false;
        sendFlag = true;
    }*/

    // The first send!
    if(sendFlag == false){ // to resend!
        //object.seq_no = getNextSeq();
        //strncpy(object.bytes, sbytes, N);
        doSend();
        timer = 0;
        cout<<"ReSent: "<<object.bytes<<" with seq no: "<<object.seq_no<<endl;
        senderTurn = false;
        sendFlag = true;
    }

    if(senderTurn == true){
            object.seq_no = getNextSeq();
            strncpy(object.bytes, sbytes, N);
            timer = 0;
            int r = doSend();
            if(!r) 
            {
                sendFlag= true;
                cout<<"Sent: "<<object.bytes<<" with seq no: "<<object.seq_no<<endl; 
                senderTurn = false;
            }
            else{ // sneder timeout!
                cout<<"TImeout : "<<object.bytes<<" with seq no: "<<object.seq_no<<endl; 
            
                senderTurn=true;
                sendFlag=false; // resnd!
            }
    }
            
} // if sender turn = true;


void recvFun(){
    Data r,s;
    event_type event;

    if(senderTurn==false){
            int stat = doRecv(&r);
            if(stat!=0){ // receive ack timedout.
                cerr<<"Ack timeout: "<<expectedSn<<endl;
                sendFlag = false;
                senderTurn = true;
                return;
            }
            if(r.isAck && r.ack_no == expectedSn){ // correctly recieved the message!
                cerr<<"Correct ack!: "<<expectedSn<<endl;
                /*if(STARTCONN){
                    STARTCONN = false;
                    object.fl2 = false;
                }*/
                sendFlag = true;
                setNextExp(); // increment expected seq no.
            }
            else {sendFlag = false;
                cerr<<"Unordered ack! ack!: "<<r.ack_no<<", exepcted: "<<expectedSn<<endl;
             }
            // else need to resend packet same packet!
            
            senderTurn = true;
        
    }
}
