/**
 * Download a file using GET request.
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
#include <pthread.h>
#include <signal.h>
#include <cmath>
#include <vector>
#include <sys/time.h>
#include <sstream>
#include <chrono>
#include <netdb.h>          /* for gethostbyname() */

#define BUF_SIZE 5000

const std::string oDir = "resp";

char buf[BUF_SIZE+1];

using namespace std;

int parseHeader(std::string resp);
std::string stripHeader(std::string r);
int getHeaderLen(std::string r);

int main(int argc, char **argv){
    bzero(buf, BUF_SIZE+1);
    std::cout << "Usage: ./a.out <URL: http(s)://<>\n";
    int mySocket = socket(AF_INET, SOCK_STREAM, 0);
    if(mySocket < 0) {std::cerr<<"Error, Socket Creation failed!"<<std::endl; exit(1);}

    if(argc!=2) {std::cerr<<"Pls enter IP + file path of object to fetch!"; exit(1);}

    // Getting IP from host!
    struct hostent *he;
    struct in_addr **addr_list;  
    char ip[100];
    char hostname[50];
    bzero(hostname, 50);
    string url(argv[1]);
    int p = url.find_first_of(":");
    url = url.substr(p+3);
    p = url.find_first_of("/");
    strcpy(hostname, url.substr(0, p).c_str());
    url = url.substr(p);


    if ( (he = gethostbyname( hostname ) ) == NULL){
        cerr<<"Cound fimd hostname: "<< hostname<< "!!\n";
        close(mySocket);
        exit(1);
    }
    addr_list = (struct in_addr **) he->h_addr_list;
 
    strcpy(ip , inet_ntoa(*addr_list[0]) );
    
    cout<<"ip::: >>>> "<<ip<<endl;

    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(80);

    if (connect(mySocket , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("connect error\n");
        exit(1);
    }

    std::string req("");
    req.append("GET ");
    req.append(url);
    req.append(" HTTP/1.1\r\n");
    req.append("Host: ");
    req.append(hostname);
    req.append("\r\n\r\n");

    std::cout<<"dbg: sending req to server: "<< req << std::endl;

    if(send(mySocket, req.c_str(), strlen(req.c_str()), 0)<0){
        std::cout<<"Send failed!";
        close(mySocket);
        exit(1);
    }

    // wiat for response now!

    p = url.find_last_of("/");
    string f_name = url.substr(p+1);
    if(f_name=="") f_name = "dl_file";
    std::ofstream oFile(oDir+"/"+f_name, std::ios::binary);

    int recvMsgSize;
    if( (recvMsgSize = recv(mySocket, buf , sizeof buf , 0)) < 0){
        cerr<<"RecvFailed outside loop\n";
        close(mySocket);
        exit(1);
    }

    int sz = parseHeader(std::string(buf))+getHeaderLen(std::string(buf));
    int headerlen = getHeaderLen(std::string(buf));
    int cur_cont_size = recvMsgSize - headerlen;

    char bufD[BUF_SIZE+1];
    bzero(bufD,BUF_SIZE+1);
    memcpy(bufD, buf+headerlen, cur_cont_size);
    bzero(buf, BUF_SIZE+1);
    memcpy(buf, bufD, cur_cont_size);

    
    int gotSize = cur_cont_size;
    sz-= headerlen;

    cout<<">>>>>>>>>>>> tot: "<<sz<<", got: "<<gotSize<<"\n";
    
    //oFile.write(buf, recvMsgSize - headerlen);
    oFile.write(buf, cur_cont_size);

    cout<<"wrote: "<<buf<<endl;
        
    while(gotSize < sz){
        
        bzero(buf, BUF_SIZE+1);

        if( (recvMsgSize = recv(mySocket, buf , sizeof buf , 0)) < 0){
            cerr<<"RecvFailed outside loop\n";
            close(mySocket);
            exit(1);
        }
        gotSize+=recvMsgSize;
    
        oFile.write(buf, recvMsgSize);  
            cout<<">>>>>>>>> tot: "<<sz<<", got: "<<gotSize<<"\n";

            cout<<"wrote: "<<buf<<endl;
  
    }

    cout<<"EOT"<<endl;

    close(mySocket);
    oFile.close();    

    return 0;
} // main

int parseHeader(std::string r){ // return content-size! tL is useless as of now
    std::string cur;
    std::istringstream resp(r);
    
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
