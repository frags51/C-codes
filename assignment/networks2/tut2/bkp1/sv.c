// usae: .file port
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/time.h> 
#include <netinet/in.h>
#include <sys/types.h> 
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

const int N = 2;

int clCount = 0;

int *clId;

int main(int argc, char *argv[])
{
    clId = (int *) malloc(N*sizeof(int));
    char buffer[2048] = {0};
    int read_val;
    struct sockaddr_in server_address;
    char* welcome = "hello server here";
    char *recMsgsBuf[N];
    char input[1024];
    int length = sizeof(server_address);
    int socket_id = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_id  == 0)
    {
        printf("Socket Error\n");
    }
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(atoi(argv[1]));
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(socket_id, (struct sockaddr*) &server_address, sizeof(server_address)) < 0)
    {
        printf("bind failed \n");
    }

    int status = listen(socket_id, N);
    if(status < 0)
    {
        printf("listen error");
    }
    printf("Waiting for %d clients to connect\n", N);
    while(1){
        int incoming_socket = accept(socket_id, (struct sockaddr *)&server_address,  
                       (socklen_t*)&length);
        if ((incoming_socket  <0) )
        { 
            printf("error in accept\n");
        }
        else printf(">> COnnnected to id: %d\n", incoming_socket); 
        clId[clCount++] = incoming_socket;
        if(clCount == N) break;
    } // while 


    while(1){ // whiel inf loop
    bzero(buffer, 2048);
    
    
    char tempBuf[128];
    bzero(tempBuf, 128);
    
    for(int i = 0; i<clCount; i++){ // receive from all.
        read_val = recv(clId[i], tempBuf, 128,0);
        char buf2[7];
        sprintf(buf2, "\n%d: ", clId[i]);
        strcat(buffer, buf2);        
        strcat(buffer, tempBuf);
        strcat(buffer, "\n");
    }

    //send to all: 

    for(int i = 0; i<clCount; i++){ // receive from all.
        send(clId[i], buffer, strlen(buffer), 0);
    }

    bzero(buffer, 2048);
    } // while inf loop
    // send to all


    for(int i=0; i<clCount; i++) close(clId[clCount]);
    close(socket_id);
    
    free(clId);


}