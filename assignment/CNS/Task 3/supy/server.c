#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#define MAX 1024
#define PORT 3561
#define N 7
int activeClients[N];
void *ThreadMain(void *arg); /* Main program of a thread */
struct ThreadArgs
{                 /* Structure of arguments to pass to client thread */
    int clntSock; /* socket descriptor for client */
};
int clntSocks[N];

int main(int argc, char *argv[])
{
    int servSock;                  /* Socket descriptor for server */
	struct sockaddr_in client[N];
    socklen_t len = sizeof(struct sockaddr_in);
    pthread_t threadID;            /* Thread ID from pthread_create()*/
    struct sockaddr_in servaddr,cli;
    struct ThreadArgs * threadArgs[N]; /* Pointer to argument structure for thread */
    


    servSock = socket(AF_INET, SOCK_STREAM, 0); 
    if (servSock == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    }

    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
  
    if ((bind(servSock, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("socket bind failed...\n"); 
        exit(0); 
    } 
  
    if ((listen(servSock, 5)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } 
    else
        printf("Server listening..\n"); 
    len = sizeof(cli); 
    for(int i = 0;i<N;i++) activeClients[i] = 0;
    int i = 0;
    for(;;)
    { /* Run forever */
        
        if( (clntSocks[i] = accept(servSock, (struct sockaddr*)&cli, (socklen_t *)&len)) <0){
            printf("Not accepting client %d\n",i+1);
        }
        activeClients[i] = 1;
        //printf("SOCKID : %d\n",clntSocks[i]);
        /* Create separate memory for client argument */
        if ((threadArgs[i] = (struct ThreadArgs *)malloc(sizeof(struct ThreadArgs))) == NULL){
            printf("ERROR in malloc\n");
            exit(1);
        }
        printf("Now at %d\n",i);
        threadArgs[i]->clntSock = clntSocks[i];
        /* Create client thread */
        if (pthread_create(&threadID, NULL, ThreadMain, (void *)threadArgs[i]) != 0){
            printf("ERROR in pthread create\n");
            exit(1);
        }
        i= (i+1)%N;
    }

    /* NOT REACHED */
}


void *ThreadMain(void *threadArgs)
{
    int clntSock;                   /* Socket descriptor for client connection */
    pthread_detach(pthread_self()); /* Guarantees that thread resources are deallocated upon return */
    /* Extract socket file descriptor from argument */
    clntSock = ((struct ThreadArgs *)threadArgs)->clntSock;


    /***********/
    char buff[MAX]; 
    int n;
        for (;;) { 
            bzero(buff, MAX); 
            read(clntSock, buff, sizeof(buff));

            printf("%s\n",buff); 
            n = 0;    
            for(int i = 0;i<N;i++){
                printf("%d", activeClients[i]);
                if(strncmp(buff,"exit",4)==0) break;
                if(activeClients[i]==1 && clntSocks[i]!=clntSock) {
                    write(clntSocks[i], buff, sizeof(buff));
                }
            }

    
            if (strncmp("exit", buff, 4) == 0) { 
                printf("Client#%d Exit...\n",clntSock); 
                activeClients[clntSock] = 0;
                close(clntSock);
                break; 
            }

    }



    /************/


    free(threadArgs); /* Deallocate memory for argument */
    //HandleTCPClient(clntSock);
    return (NULL);
}
