#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h> 
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>

void recMsg(int client){
    char buf[1024];
    int recSize;


    printf("\nEnter message to send (<1024 chars)!: ");
    scanf("%s", buf);
    
    // send message back!
    if (send(client, buf, strlen(buf), 0) != strlen(buf))
        {perror("Cleint: Error: msg cant be sent\n"); exit(1);} 
    printf("cl: sent msg: %s!\n", buf);
    if ((recSize = recv(client, buf, recSize, 0)) < 0)
        {perror("CLient: Error: msg cant be recd\n"); exit(1);}

    printf("Client: Got Message:  ");
    
    while (recSize > 0) /* zero indicates end of transmission */
    {
    /* Echo message back to screen */
    printf("%s", buf);
    /* See if there is more data to receive */
    if ((recSize = recv(client, buf, recSize, 0)) < 0)
        {perror("Client: Error: msg cant be recd\n"); exit(1);}
    }
   

} // recMsg ends

int main(void){
    int servSock, clSock;
    int port = 3506; // server listens on this port
    // create sock
    if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        {perror("Serv: Error: Socket cant'be created\n"); exit(1);}
    
    struct sockaddr_in servAddr;

    servAddr.sin_family = AF_INET; 
    servAddr.sin_addr.s_addr= inet_addr("127.0.0.1");
    servAddr.sin_port = htons(port); 

    if (connect(servSock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 

//    int clfd;
//    struct sockaddr_in clientAddr;

    for(;;){ // loop
//        int clLen = sizeof(clientAddr);

        printf("Client: Server is now connected!\n");

        recMsg(servSock);
    }

    return 0;
}
