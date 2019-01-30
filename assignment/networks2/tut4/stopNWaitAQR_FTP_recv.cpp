/**
 * STOP AND WAIT AQR. (on localhost)
 * usage: ./file <port to run on> <port to send ack to>
 * port to run on == port to send to of sender and vice-versa.
 * Receiver. (client?) sends and waits for ACK. then again sends and waits for ack.
*/
#include "ftp_aqr.h"

bool DISCONNECT = false;

int mySocket;
sockaddr_in toAddr;


bool sendFlag = true;

bool senderTurn = false;

ofstream oFile("recd.txt", ios::binary);


Data object;

void senderFun();
void recvFun();
void storeDataToFile(const ofstream& outfile);

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
    if(sendto(mySocket, &object, echoStringLen, 0, (sockaddr *)&toAddr, sizeof(toAddr))<0){
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
    tv.tv_sec = 1;
    tv.tv_usec = TIMEOUT;
    if (setsockopt(mySocket, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
        perror("Error");
    }

    toAddr;
    toAddr.sin_family = AF_INET;
    toAddr.sin_port = htons(toPort);
    toAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    object.isAck= true;
    object.ack_no = expectedSn;
    
    auto start = std::chrono::system_clock::now();

    while(!DISCONNECT){
        recvFun();        
        senderFun();
    }
    auto end = std::chrono::system_clock::now();
    auto elapsed1 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    cout<<"Time taken: "<<elapsed1.count()<<endl;
    oFile.close();
    close(mySocket);

} //main

void senderFun(){

    event_type event;

    // The first send!
    if(sendFlag == false){ // to resend!
        //object.seq_no = getNextSeq();
        //strncpy(object.bytes, sbytes, N);
        doSend();
        timer = 0;
        cout<<"ReSent ack" << " with ack no: "<<object.ack_no<<endl;
        senderTurn = false;
        sendFlag = true;
    }

    if(senderTurn == true){
            object.seq_no = getNextSeq();
            //strncpy(object.bytes, sbytes, N);
            timer = 0;
            int r = doSend();
            if(!r) 
            {
                cout<<"Sent: "<<object.bytes<<" with ack no: "<<object.ack_no<<endl; 
                senderTurn = false;
            }
            else{ // sneder timeout!
                cout<<"TImeout : "<<object.bytes<<" with ack no: "<<object.ack_no<<endl; 
            
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
                sendFlag = false;
                senderTurn = true;
                return;
            }
            if(r.disconnect == true){
                // need to close connection!
                cerr<<"conn close msg recd\n";
                DISCONNECT = true;
                return;
            }
            cout<<">>>>>>> recd: "<<r.bytes<<" , dsq no: "<< r.seq_no<<endl;
            cerr<<">>> : "<<(r.bytes[N-1])<<endl;
            cout<<"Expecting sqno:: "<<(expectedSn-1)<<endl;
            if(r.seq_no == expectedSn-1 && (r.csum==checkSum(r.bytes, r.bytes_length))) { // correctly recieved the message!
                
                setNextExp(); // increment expected seq no.
                object.isAck = true;
                object.ack_no = r.seq_no+1;
                cout<<"correct rec, writing to file\n";
                oFile.write( r.bytes, r.bytes_length);
            }
            else sendFlag = false;
            // else need to resend packet same packet!
            
            senderTurn = true;
        
    }
}
