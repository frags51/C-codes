#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h> 
#include <unistd.h>
#include <stdlib.h>

void recMsg(int client){
    char buf[1024];
    int recSize;
    printf("Serv : Waiting to rcv!\n");
    if ((recSize = recv(client, buf, recSize, 0)) < 0)
        {perror("Serv: Error: msg cant be recd\n"); exit(1);}

    printf("Serv: Got Message:  ");
    printf("%d, %s",recSize, buf);
    
    while (recSize > 0) /* zero indicates end of transmission */
    {
    /* Echo message back to screen */
    printf("%d, %s",recSize, buf);
    /* See if there is more data to receive */
    if ((recSize = recv(client, buf, recSize, 0)) < 0)
        {perror("Serv: Error: msg cant be recd\n"); exit(1);}
    }

    printf("\nEnter message to send (<1024 chars)!: ");
    scanf("%s", buf);
    
    // send message back!
    if (send(client, buf, strlen(buf), 0) != strlen(buf))
        {perror("Serv: Error: msg cant be sent\n"); exit(1);}    

} // recMsg ends

int main(void){
    int servSock, clSock;
    int port = 3506; // server listens on this port
    // create sock
    if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        {perror("Serv: Error: Socket cant'be created\n"); exit(1);}
    
    struct sockaddr_in servAddr;

    servAddr.sin_family = AF_INET; 
    servAddr.sin_addr.s_addr= INADDR_ANY;
    servAddr.sin_port = htons(port); 

    // bind
      if(bind(servSock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
        {perror("Serv: Error: Bind failed\n"); exit(1);}

    if (listen(servSock, 2) < 0)
        {perror("Serv: Error: Listen() failed\n"); exit(1);}

    int clfd;
    struct sockaddr_in clientAddr;

    for(;;){ // loop
        int clLen = sizeof(clientAddr);
        printf("Serv: DBG: waitin for acc\n");
        if ((clSock = accept(servSock, (struct sockaddr *) &clientAddr, &clLen)) < 0)
            {perror("Serv: Error: accept() failed\n"); exit(1);}

        printf("Serv: Client now connected!\n");

        recMsg(clSock);
    }



    return 0;
}
