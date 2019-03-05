/**
 * (a) Download a file using GET request.
 */
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
#include <thread>
#include <signal.h>
#include <cmath>
#include <vector>
#include <sys/time.h>
#include <sstream>
#include <chrono>
#include <netdb.h>          /* for gethostbyname() */
#include "port.h"

#define BUF_SIZE 5000
const std::string oDir = "resp";

char buf[BUF_SIZE+1];

using namespace std;

int parseHeader(std::string resp);
std::string stripHeader(std::string r);
int getHeaderLen(std::string r);

int parseReq(char *req, char*resp, int clSock);
int parseReqGET(char *req, char*resp, int clSock);
int parseReqHEAD(char *req, char*resp, int clSock);
int parseReqPOST(char *req, char*resp, int clSock);
void threadMain(int incoming_socket);

int main(int argc, char **argv){
    bzero(buf, BUF_SIZE+1);
    std::cout << "Usage: ./a.out <URL: http(s)://<>\n";

    int clSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clSocket < 0) {std::cerr<<"Error, CL Socket Creation failed!"<<std::endl; exit(1);}
    struct sockaddr_in forCL;
    forCL.sin_addr.s_addr = htonl(INADDR_ANY);
    forCL.sin_family = AF_INET;
    forCL.sin_port = htons(PORT);
    if(bind(clSocket, (struct sockaddr*) &forCL, sizeof(forCL)) < 0)
    {
        printf("clSocket bind failed \n");
        close(clSocket);
        exit(1);
    }
    int status = listen(clSocket, 3);
    if(status < 0)
    {
        printf("listen error");
        close(clSocket);
        exit(1);
    }
    else{ cerr<<"Listening for connections!\n";}
    long length;
    while(1){
        int incoming_socket = accept(clSocket, (struct sockaddr *)&forCL,  
                       (socklen_t*)&length);
        if ((incoming_socket  <0) )
        { 
            printf("error in accept\n");
        }
        else printf(">> COnnnected to id: %d\n", incoming_socket); 

        // now recv req from socket!

        
        std::thread t(threadMain, incoming_socket);
        t.detach();
        //exit(0);

    } // while 

    close(clSocket);
    

    //if(argc!=2) {std::cerr<<"Pls enter IP + file path of object to fetch!"; exit(1);}



    // wiat for response now!    

    return 0;
} // main

int parseHeader(std::string r){ // return content-size! tL is useless as of now
    std::string cur;
    std::istringstream resp(r);
    
    if(r.find("Content-Length")==string::npos) {
        cerr<<"No content length!"<<endl;
        return 0;
        //exit(1);
    }

    int tL = 0;

    while(true){
        std::getline(resp, cur);
        if(cur.compare("\r") == 0) {/*return tL+2; */ /*For last \r\n*/}
        int v; 
        if( (v = cur.find("Content-Length: "))==string::npos) {
            //cout<<";GheadeR: "<<cur<<", Lngt: "<<(cur.length()+1)<<endl;
            tL+= cur.length()+1;
        }
        else {
            //cout<<"--> Got header: "<<cur<<", Ln:"<<(cur.length()+1)<<endl;
            
            std::string len_s = cur.substr(16);
            cout<<"LEN_PARSE: "<<len_s<<"\n";
            return stoi(len_s);
            tL+= stoi(len_s);
            tL+=cur.length()+1;
        }
    }
    
}

std::string stripHeader(std::string r){
    int p = r.find("\r\n\r\n");
    return  r.substr(p+4);
}

int getHeaderLen(std::string r){
    int p = r.find("\r\n\r\n");
    return p+4; // +1 cuz 0 indexed???;
}

int parseReq(char *req, char*resp, int clSock){
    string r(req);
    if(r.at(0)=='G') parseReqGET(req, resp, clSock);
    else if(r.at(0) == 'H') parseReqHEAD(req, resp, clSock);
    return 0;
}

int parseReqGET(char *req, char*resp, int clSock){
    string r(req);
    std::istringstream r_stream(r);
    string cur;
    string host_name;
    // find Host
    while(true){
        std::getline(r_stream, cur);
        if(cur.compare("\r") == 0) {cerr<<"Host: not fpund in GET!\n"; break;}
        int v; 
        if( (v = cur.find("Host: "))==string::npos) {
            //cout<<";GheadeR: "<<cur<<", Lngt: "<<(cur.length()+1)<<endl;
            continue;
        }
        else {
            //cout<<"--> Got header: "<<cur<<", Ln:"<<(cur.length()+1)<<endl;
            auto pd = cur.find_first_of("\r\n");
            host_name = cur.substr(v+6, pd-v-6);
            cerr<<"Found host: "<<host_name<<endl;
            break;
        }
    }

    // Getting IP from host!
    struct hostent *he;
    struct in_addr **addr_list;  
    char ip[100];
    
    char hn[50]; 
    strcpy(hn, host_name.c_str());

    if ( (he = gethostbyname(hn ) ) == NULL){
        cerr<<"Cound fimd hostname: "<< hn<< " " << (strlen(hn)-1) << "!!\n";
        cerr<<"b: "<<hn[strlen(hn)-1];
        close(clSock);
        exit(1);
    }
    addr_list = (struct in_addr **) he->h_addr_list;
 
    strcpy(ip , inet_ntoa(*addr_list[0]) );
    
    cerr<<"Found ip::: >>>> "<<ip<<endl;

    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(80);

    int mySocket = socket(AF_INET, SOCK_STREAM, 0);

    if(mySocket < 0) {std::cerr<<"Error, Socket Creation failed!"<<std::endl; exit(1);}

    if (connect(mySocket , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("connect error inside parseGetReq\n");
        exit(1);
    }

    std::cout<<"dbg: sending req to server: "<< req << std::endl;
    if(send(mySocket, req, strlen(req), 0)<0){
        std::cout<<"Send failed inside GETREQ!";
        close(mySocket);
        exit(1);
    }

    string url = r.substr(4); // After "GET "
    int _p = url.find_first_of(" ");
    url = url.substr(0, _p);
    cerr<<"Got URL to GET: "<<url<<endl;
    int p = url.find_last_of("/");
    string f_name = url.substr(p+1);
    if(f_name=="") f_name = "dl_file";
    //std::ofstream oFile(oDir+"/"+f_name, std::ios::binary);

    int recvMsgSize;
    if( (recvMsgSize = recv(mySocket, buf , sizeof buf , 0)) < 0){
        cerr<<"RecvFailed outside loop\n";
        close(mySocket);
        exit(1);
    }
    else{
        cerr<<"rexcd Init: \n";
        //cerr<<buf<<endl;
    }

    int sz = parseHeader(std::string(buf))+getHeaderLen(std::string(buf));
    int headerlen = getHeaderLen(std::string(buf));
    int cur_cont_size = recvMsgSize;

    //cerr<<"Full resp: "<<buf<<endl; 

    int gotSize = cur_cont_size;
    //sz-= headerlen;

    cout<<">>>>>>>>>>>> tot: "<<sz<<", got: "<<gotSize<<"\n";
    
    //oFile.write(buf, recvMsgSize - headerlen);
    if(send(clSock, buf, recvMsgSize, 0)<0){
        cerr<<"Failed to send resp back to client out loop\n";
    }
    else{
        cerr<<"Sent to client: "<<(buf)<<endl;
    }
    //oFile.write(buf, cur_cont_size);

    //cout<<"wrote: "<<buf<<endl;
        
    while(gotSize < sz){
        
        bzero(buf, BUF_SIZE+1);

        if( (recvMsgSize = recv(mySocket, buf , sizeof buf , 0)) < 0){
            cerr<<"RecvFailed inside loop\n";
            close(mySocket);
            exit(1);
        }
        gotSize+=recvMsgSize;
    
        //oFile.write(buf, recvMsgSize);  
        if(send(clSock, buf, recvMsgSize, 0)<0){
            cerr<<"Failed to send resp back to client out loop\n";
        }
        else{
            //cerr<<"Sent to client: "<<(buf)<<endl;
        }
        
            cout<<">>>>>>>>> tot: "<<sz<<", got: "<<gotSize<<"\n";
  
    }

    cout<<"EOT"<<endl;
    close(clSock);
    close(mySocket);
} // reqGet


int parseReqHEAD(char *req, char*resp, int clSock){
    string r(req);
    std::istringstream r_stream(r);
    string cur;
    string host_name;
    // find Host
    while(true){
        std::getline(r_stream, cur);
        if(cur.compare("\r") == 0) {cerr<<"Host: not fpund in GET!\n"; break;}
        int v; 
        if( (v = cur.find("Host: "))==string::npos) {
            //cout<<";GheadeR: "<<cur<<", Lngt: "<<(cur.length()+1)<<endl;
            continue;
        }
        else {
            //cout<<"--> Got header: "<<cur<<", Ln:"<<(cur.length()+1)<<endl;
            auto pd = cur.find_first_of("\r\n");
            host_name = cur.substr(v+6, pd-v-6);
            cerr<<"Found host: "<<host_name<<endl;
            break;
        }
    }

    // Getting IP from host!
    struct hostent *he;
    struct in_addr **addr_list;  
    char ip[100];
    
    char hn[50]; 
    strcpy(hn, host_name.c_str());

    if ( (he = gethostbyname(hn ) ) == NULL){
        cerr<<"Cound fimd hostname: "<< hn<< " " << (strlen(hn)-1) << "!!\n";
        cerr<<"b: "<<hn[strlen(hn)-1];
        close(clSock);
        exit(1);
    }
    addr_list = (struct in_addr **) he->h_addr_list;
 
    strcpy(ip , inet_ntoa(*addr_list[0]) );
    
    cerr<<"Found ip::: >>>> "<<ip<<endl;

    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(80);

    int mySocket = socket(AF_INET, SOCK_STREAM, 0);

    if(mySocket < 0) {std::cerr<<"Error, Socket Creation failed!"<<std::endl; exit(1);}

    if (connect(mySocket , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("connect error inside parseGetReq\n");
        exit(1);
    }

    std::cout<<"dbg: sending req to server: "<< req << std::endl;
    if(send(mySocket, req, strlen(req), 0)<0){
        std::cout<<"Send failed inside GETREQ!";
        close(mySocket);
        exit(1);
    }

    string url = r.substr(4); // After "GET "
    int _p = url.find_first_of(" ");
    url = url.substr(0, _p);
    cerr<<"Got URL to GET: "<<url<<endl;
    int p = url.find_last_of("/");
    string f_name = url.substr(p+1);
    if(f_name=="") f_name = "dl_file";
    //std::ofstream oFile(oDir+"/"+f_name, std::ios::binary);

    int recvMsgSize;
    if( (recvMsgSize = recv(mySocket, buf , sizeof buf , 0)) < 0){
        cerr<<"RecvFailed outside loop\n";
        close(mySocket);
        exit(1);
    }
    else{
        cerr<<"rexcd Init: \n";
        //cerr<<buf<<endl;
    }

    int sz = getHeaderLen(std::string(buf));
    int headerlen = getHeaderLen(std::string(buf));
    int cur_cont_size = recvMsgSize;

    //cerr<<"Full resp: "<<buf<<endl; 

    int gotSize = cur_cont_size;
    //sz-= headerlen;

    cout<<">>>>>>>>>>>> tot: "<<sz<<", got: "<<gotSize<<"\n";
    
    //oFile.write(buf, recvMsgSize - headerlen);
    if(send(clSock, buf, recvMsgSize, 0)<0){
        cerr<<"Failed to send resp back to client out loop\n";
    }
    else{
        cerr<<"Sent to client: "<<(buf)<<endl;
    }
    //oFile.write(buf, cur_cont_size);

    //cout<<"wrote: "<<buf<<endl;
        
    while(gotSize < sz){
        
        bzero(buf, BUF_SIZE+1);

        if( (recvMsgSize = recv(mySocket, buf , sizeof buf , 0)) < 0){
            cerr<<"RecvFailed inside loop\n";
            close(mySocket);
            exit(1);
        }
        gotSize+=recvMsgSize;
    
        //oFile.write(buf, recvMsgSize);  
        if(send(clSock, buf, recvMsgSize, 0)<0){
            cerr<<"Failed to send resp back to client out loop\n";
        }
        else{
            cerr<<"Sent to client: "<<(buf)<<endl;
        }
        
            cout<<">>>>>>>>> tot: "<<sz<<", got: "<<gotSize<<"\n";
  
    }

    cout<<"EOT"<<endl;
    close(clSock);
    close(mySocket);
    return 0;
}
int parseReqPOST(char *req, char*resp, int clSock){
    return 0;
}

void threadMain(int incoming_socket){
    char reqFromCl[BUF_SIZE];
        if(recv(incoming_socket, reqFromCl, BUF_SIZE, 0)<0){
            cerr<<"recv from cl failed. "<<endl;
            close(incoming_socket);
            exit(1);
        }
        else{
            cerr<<"DBG: Received from client: "<<endl;
            cerr<<reqFromCl<<endl;
        }
    char response[BUF_SIZE];
        
    parseReq(reqFromCl, response, incoming_socket);
}