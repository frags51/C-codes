/**
 * Assumption : Send && Recver's bufffer are of same size
 * == const int N
 * 
 * STOP AND WAIT AQR. (on localhost)
 * usage: ./file <port to run on> <port to send to> <filename>
 * port to run on == port to send to of receiver and vice-versa.
 * Sender. (server?) sends and waits for ACK. then again sends and waits for ack.
 */
#include "ftp_aqr.h"

bool STARTCONN = true; 

int mySocket;
sockaddr_in toAddr;


bool sendFlag = true; // True, if you DO NOT need to resend same mesage. 

bool senderTurn = true;


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

char *getDataFromFile( ifstream&);

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

    size_t echoStringLen = sizeof(object);
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
    if(argc!=4) {
        cerr<<"Incorrect number of arguments. Need 3!"<<endl;
        exit(1);
    }

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
    tv.tv_sec = 1;
    tv.tv_usec = TIMEOUT;
    if (setsockopt(mySocket, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
        perror("Error");
    }

    toAddr;
    toAddr.sin_family = AF_INET;
    toAddr.sin_port = htons(toPort);
    toAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    ifstream inf(argv[3], ios::binary);

    while(true){
        char* c;
        if(!sendFlag || (c=(getDataFromFile(inf))) !=NULL){
            senderFun(c);
            recvFun();
        }
        else break;   
    }
    object.disconnect = true;
    doSend();
    close(mySocket);
} //main

void senderFun(char sbytes[N]){

    event_type event;


    // The first send!
    if(sendFlag == false){ // to resend!
        //object.seq_no = getNextSeq();
        //strncpy(object.bytes, sbytes, N);
        doSend();
        timer = 0;
        cout<<"ReSent: "<<" with seq no: "<<object.seq_no<<endl;
        senderTurn = false;
        sendFlag = true;
    }

    if(senderTurn == true){
            object.seq_no = getNextSeq();
            memcpy(object.bytes, sbytes, object.bytes_length);
            timer = 0;
            int r = doSend();
            if(!r) 
            {
                sendFlag= true;
                cout<<"Sent: "<<object.bytes<<" with seq no: "<<object.seq_no<<endl; 
                senderTurn = false;
                free(sbytes);
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

//ofstream off("recd2.txt",ios::binary);

char *getDataFromFile(ifstream& infile){
    // current position in file.

    if(infile.eof()) return NULL;

    char *b = (char *)malloc(N*sizeof(char));
    bzero(b, N*sizeof(char));
    infile.read(b, N);
    object.bytes_length = infile.gcount();
    object.csum = checkSum(b, object.bytes_length);
    cerr<<"::: Got checksum: "<<object.csum<<endl;
    //off.write(b, object.bytes_length);
    return b;
}
