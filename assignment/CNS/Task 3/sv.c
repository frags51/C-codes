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

#include <openssl/ssl.h>

void *threadMain(void *arg);

const int N = 1;
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
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    SSL_METHOD *method = TLSv1_2_server_method();
    SSL_CTX *ctx = SSL_CTX_new(method);

    
    clId = (int *) malloc(N*sizeof(int));
    tId = (pthread_t *) malloc(N*sizeof(pthread_t ));
    
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
    if (pthread_create (&(tId[clCount-1]), NULL, threadMain, (void *) incoming_socket) != 0) {perror("thread create fail\n"); exit(1);} 


        if(clCount == N) break;
    } // while 


    /*
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
*/

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
    
        for(int i=0; i<clCount; i++) close(clId[clCount]);

    close(socket_id);
    
    free(clId);


}

void *threadMain(void *arg){
    int clientId = (pthread_t) arg;
    
    pthread_detach(pthread_self());
    int flag[15] = {0};
    char buffer[2048];
    while(1){
        bzero(buffer, 2048);
    
    
    char tempBuf[128];
    bzero(tempBuf, 128);
    
     // receive from all.
        int read_val;
        if(!flag[clientId])read_val = recv(clientId, tempBuf, 128,0);
        if(read_val <1) flag[clientId] = 1;;
        printf("read! ffom %d\n", clientId);
        char buf2[7];
        sprintf(buf2, "\n%d: ", clientId);
        strcat(buffer, buf2);        
        strcat(buffer, tempBuf);
        strcat(buffer, "\n");
    

    //send to all: 

    for(int i = 0; i<clCount; i++){ // receive from all.
        if(clId[i]!=clientId && !flag[clientId]) send(clId[i], buffer, strlen(buffer), 0);
    }

    bzero(buffer, 2048);
    }

}
