
#include <iostream>
#include <fstream>
#include <string>
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
#include <sstream>
#include <chrono>

#define BUF_SIZE 5000

const std::string oDir = "resp/";

char buf[BUF_SIZE];

using namespace std;

int parseHeader(std::string resp);
std::string stripHeader(std::string r);

int main(int argc, char **argv){
    bzero(buf, BUF_SIZE);
    std::cout << "Usage: ./a.out IP FilePath HostName\n";
    int mySocket = socket(AF_INET, SOCK_STREAM, 0);
    if(mySocket < 0) {std::cerr<<"Error, Socket Creation failed!"<<std::endl; exit(1);}

    if(argc!=4) {std::cerr<<"Pls enter IP + file path of object to fetch!"; exit(1);}

    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_family = AF_INET;
    server.sin_port = htons(80);

    if (connect(mySocket , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("connect error\n");
        exit(1);
    }

    std::string req("");
    req.append("GET ");
    req.append(argv[2]);
    req.append(" HTTP/1.1\r\n");
    req.append("Host: ");
    req.append(argv[3]);
    req.append("\r\n\r\n");

    std::cout<<"dbg: sending req to server: "<< req << std::endl;

    if(send(mySocket, req.c_str(), strlen(req.c_str()), 0)<0){
        std::cout<<"Send failed!";
        close(mySocket);
        exit(1);
    }

    // wiat for response now!

    std::ofstream oFile(oDir+"f.txt", std::ios::binary);

    int recvMsgSize;
    if( (recvMsgSize = recv(mySocket, buf , sizeof buf , 0)) < 0){
        cerr<<"RecvFailed outside loop\n";
        close(mySocket);
        exit(1);
    }

    int sz = parseHeader(std::string(buf));
    int gotSize = recvMsgSize;
    cout<<">>>>>>>>>>>> tot: "<<sz<<", got: "<<gotSize<<"\n";

    char bufD[BUF_SIZE];
    strcpy(bufD,(stripHeader(std::string(buf))).c_str() );
    
    bzero(buf, BUF_SIZE);
    memcpy(buf, bufD, BUF_SIZE);

    oFile.write(buf, recvMsgSize);
    cout<<"wrote: "<<buf<<endl;
        
    while(gotSize < sz){
        
        bzero(buf, BUF_SIZE);

        if( (recvMsgSize = recv(mySocket, buf , sizeof buf , 0)) < 0){
            cerr<<"RecvFailed outside loop\n";
            close(mySocket);
            exit(1);
        }
        gotSize+=recvMsgSize;
    
        oFile.write(buf, recvMsgSize);    
        cout<<">>>>>>>>> tot: "<<sz<<", got: "<<gotSize<<"\n";
    }

    cout<<"EOT"<<endl;

    close(mySocket);
    oFile.close();    

    return 0;
} // main

int parseHeader(std::string r){
    std::string cur;
    std::istringstream resp(r);
    
    int tL = 0;

    while(true){
        std::getline(resp, cur);
        if(resp.eof() || cur.compare("\r") == 0) {return tL+2; /*For last \r\n*/}
        int v; 
        if( (v = cur.find("Content-Length: "))==string::npos) {
            //cout<<";GheadeR: "<<cur<<", Lngt: "<<(cur.length()+1)<<endl;
            tL+= cur.length()+1;
        }
        else {
            //cout<<"--> Got header: "<<cur<<", Ln:"<<(cur.length()+1)<<endl;
            
            std::string len_s = cur.substr(16);
//            cout<<"LEN_PARSE: "<<len_s<<"\n";
            tL+= stoi(len_s);
            tL+=cur.length()+1;
        }
    }
    
}

std::string stripHeader(std::string r){
    int p = r.find("\r\n\r\n");
    return r.substr(p+4);
}
