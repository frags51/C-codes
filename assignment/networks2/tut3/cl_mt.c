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
#include <signal.h>
#include <pthread.h>

#define SQRT 0
#define ADD 1
#define DIV 2
#define ANS 3

#define MAX_ARGS 40

typedef struct _msg{
    int id; // what type of function etc
    int argsize;
    float args[MAX_ARGS];
} msg_struct;




int socket_id;

void sigIntHandler(int id){
	close(socket_id);
    printf(">> Closed!\n");
    
	exit(1);
}


int main(int argc, char *argv[])
{
    char buffer[2048] = {0};
    int read_val;
    char input[1024];
    struct sockaddr_in client_address;
    struct sockaddr_in server_addr;
    socket_id = socket(AF_INET, SOCK_STREAM, 0);
    char *welcome = "hello client here";
    if(socket_id  == 0)
    {
        printf("Socket Error\n");
    }

struct sigaction handler;
	handler.sa_handler = sigIntHandler;
	sigfillset(&handler.sa_mask);
	handler.sa_flags =0;
	if (sigaction(SIGINT, &handler, 0)<0)
	{
		perror("sigaction faield!@\n");
		exit(1);
    }


    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));

    int connection = connect(socket_id, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if(connection < 0)
        printf("Connection error\n");
    else printf("Connected!\n");
    
    int op;
    printf("Enter operation you want: 0-> SQRT, 1-> ADD, 2-> DIVIDE: ");
    scanf("%d", &op);

    switch(op){
        case SQRT:{
            float num; printf("Enter number to get sqrt of: ");
            scanf("%f", &num);
            msg_struct a;
            a.id = (0);
            a.argsize = (1);
            a.args[0] = (num);
            send(socket_id, &a, sizeof(a), 0 );
            printf("DBG: sent, waitinf for response...\n");
            
            recv(socket_id, &a, sizeof(a), 0);
            printf("Recd ans: %f\n", (a.args[0]));
        }
        break;

        case ADD:{

            int num; printf("Enter number of arguments to add: ");

            scanf("%d", &num);
            float b;
            msg_struct a;            
            for(int i=0; i<num; i++) {scanf("%f", &b); a.args[i]=(b);}
            a.id = (1);
            a.argsize = (num);
            send(socket_id, &a, sizeof(a), 0 );
            
            printf("DBG: sent, waitinf for response...\n");
            
            recv(socket_id, &a, sizeof(a), 0);
            printf("Recd ans: %f\n", (a.args[0]));
        
        }
        break;

        case DIV:{
            int num; printf("Enter 2  numbers to divide: ");
            float b;
            msg_struct a;
            
            for(int i=0; i<2; i++) {scanf("%f", &b); a.args[i]=(b);}
            a.id = (2);
            a.argsize = (2);
            if(a.args[1]==0) {perror("dont div by 0"); exit(0);}
            send(socket_id, &a, sizeof(a), 0 );

            printf("DBG: sent, waitinf for response...\n");
            
            recv(socket_id, &a, sizeof(a), 0);
            printf("Recd ans: quo: %f, rem: %f\n", (a.args[0]), a.args[1]);
        
        }
        break;

        default:
        printf("Error: wrong code!\n");
        break;
    } // switch

    close(socket_id);
}
