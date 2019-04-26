#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <pthread.h>
#define MAX 1024
#define PORT 3561
void* writeMssg(void *args) 
{ 
    int sockfd = (int) args;
    char buff[MAX]; 
    int n; 
    for (;;) { 
        bzero(buff, sizeof(buff)); 
        n = 0; 
        while ((buff[n++] = getchar()) != '\n') ; 
        if(write(sockfd, buff, sizeof(buff)) == -1) {
            exit(1); 
        }
        if ((strncmp(buff, "exit", 4)) == 0) { 
            printf("Exit...\n"); 
            exit(1);
        } 
        
    } 
} 

void* readMssg(void* args){
    int sockfd = (int) args;
    char buff[MAX];
    int n;
    for(;;){
        bzero(buff,sizeof(buff));
        n = 0;
        read(sockfd,buff,sizeof(buff));
        if ((strncmp(buff, "FULL", 4)) == 0) { 
            printf("NETWORK FULL Exit...\n"); 
            exit(1);
        } 
        printf("Friend : %s",buff);
    }



}

  
int main() 
{ 
    int sockfd, connfd; 
    struct sockaddr_in servaddr, cli; 
  
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    bzero(&servaddr, sizeof(servaddr)); 
  
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(PORT); 
  
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) { 
        printf("connection with the server failed...\n"); 
        exit(0); 
    } 
    pthread_t thread_id[2];

    pthread_create(&thread_id[0], NULL,writeMssg,(void *)sockfd); 
    pthread_create(&thread_id[1],NULL,readMssg,(void*)sockfd);

    for(int i = 0;i<2;i++) pthread_join(thread_id[i],NULL);

 
  
    close(sockfd); 
} 
