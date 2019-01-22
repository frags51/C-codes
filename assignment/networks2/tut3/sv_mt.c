
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
#include <pthread.h>
#include <signal.h>
#include <math.h>

#define MAX_ARGS 40

#define SQRT 0
#define ADD 1
#define DIV 2
#define ANS 3

typedef struct _msg{
    int id; // what type of function etc
    int argsize;
    float args[MAX_ARGS];
} msg_struct;


void *threadMain(void *arg);

const int N = 40;
int socket_id;
int clCount = 0;

int *clId;
pthread_t *tId;
void sigIntHandler(int id){
    for(int i=0; i<clCount; i++) close(clId[clCount]);
	close(socket_id);
    printf(">> Closed!\n");
    
	exit(1);
}

int main(int argc, char *argv[])
{
    clId = (int *) malloc(N*sizeof(int));
    tId = (int *) malloc(N*sizeof(pthread_t ));
    
    char buffer[2048] = {0};
    int read_val;
    struct sockaddr_in server_address;
    char* welcome = "hello server here";
    char *recMsgsBuf[N];
    char input[1024];
    int length = sizeof(server_address);
    socket_id = socket(AF_INET, SOCK_STREAM, 0);
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
    //printf("Waiting for %d clients to connect\n", N);
    while(1){
        printf("main is here!\n");
        sleep(1);
        int incoming_socket = accept(socket_id, (struct sockaddr *)&server_address,  
                       (socklen_t*)&length);
        if ((incoming_socket  <0) )
        { 
            printf("error in accept\n");
        }
        else printf(">> COnnnected to id: %d\n", incoming_socket); 
        clId[clCount++] = incoming_socket;
    if (pthread_create (&(tId[clCount-1]), NULL, threadMain, (void *) incoming_socket) != 0) {perror("thread create fail\n"); exit(1);} 


        if(clCount == N) break;
    } // while 


    struct sigaction handler;
	handler.sa_handler = sigIntHandler;
	sigfillset(&handler.sa_mask);
	handler.sa_flags =0;
	if (sigaction(SIGINT, &handler, 0)<0)
	{
		perror("sigaction faield!@\n");
		exit(1);
    }

    for(;;) sleep(1);
    
    //    for(int i=0; i<clCount; i++) close(clId[clCount]);

    close(socket_id);
    
    free(clId);


}

void *threadMain(void *arg){
    int clientId = (pthread_t) arg;
    


    pthread_detach(pthread_self());
    
    msg_struct rcvBuffer;
    if ( recv(clientId, &rcvBuffer, sizeof(rcvBuffer), 0) < 0)
            perror("recvfrom() failed");

    else printf("Recvd!! %f..\n", rcvBuffer.args[0]);

    printf("blah");
    int op = (rcvBuffer.id);
    switch(op){
        case SQRT:
        
            rcvBuffer.id = (ANS);
            rcvBuffer.argsize = (1);
            int num = (rcvBuffer.args[0]);
            rcvBuffer.args[0] = (sqrt(num));
            fprintf(stderr, "setnding %f", rcvBuffer.args[0]);
            if (send(clientId, &rcvBuffer, sizeof(rcvBuffer), 0) <0 ) perror("error in send");
            perror("snet!");
        break;

        case ADD:{
            rcvBuffer.id = (ANS);
            int num = (rcvBuffer.argsize);
            
            rcvBuffer.argsize = (1);
            float sum = 0;
            for(int i=0; i<num; i++) sum+=rcvBuffer.args[i];
            rcvBuffer.args[0] = sum;
            rcvBuffer.argsize = 1;
            fprintf(stderr, "setnding %f", rcvBuffer.args[0]);
            if (send(clientId, &rcvBuffer, sizeof(rcvBuffer), 0) <0 ) perror("error in send");
            perror("snet!");
        }

        break;

        case DIV:{
            rcvBuffer.id = (ANS);
            rcvBuffer.argsize = (2);
            int n1 = rcvBuffer.args[0]; int n2 = rcvBuffer.args[1];
            int q = n1/n2;
            int r = n1 % n2;

            rcvBuffer.args[0] = q;
            rcvBuffer.args[1] = r;
            fprintf(stderr, "setnding %f", rcvBuffer.args[0]);
            if (send(clientId, &rcvBuffer, sizeof(rcvBuffer), 0) <0 ) perror("error in send");
            perror("snet!");
        }

        break;
    }
    close(clientId);
}