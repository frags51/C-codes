#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
 
#define FAIL    -1
 
int OpenConnection(const char *hostname, int port)
{   int sd;
    struct hostent *host;
    struct sockaddr_in addr;
 
    
    sd = socket(PF_INET, SOCK_STREAM, 0);
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    if ( connect(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
    {
        close(sd);
        perror(hostname);
        abort();
    }
    return sd;
}
 
SSL_CTX* InitCTX(void)
{   SSL_METHOD *method;
    SSL_CTX *ctx;
 
    OpenSSL_add_all_algorithms();  /* Load cryptos, et.al. */
    SSL_load_error_strings();   /* Bring in and register error messages */
    method = TLSv1_2_client_method();  /* Create new client-method instance */
    ctx = SSL_CTX_new(method);   /* Create new context */
    if ( ctx == NULL )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}

void LoadCertificates(SSL_CTX* ctx, char* CertFile, char* KeyFile)
{
    /* set the local certificate from CertFile */
    if ( SSL_CTX_use_certificate_chain_file(ctx, CertFile) <= 0 )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    /* set the private key from KeyFile (may be the same as CertFile) */
    if ( SSL_CTX_use_PrivateKey_file(ctx, KeyFile, SSL_FILETYPE_PEM) <= 0 )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    /* verify private key */
    if ( !SSL_CTX_check_private_key(ctx) )
    {
        fprintf(stderr, "Private key does not match the public certificate\n");
        abort();
    }
}
 
 
void ShowCerts(SSL* ssl)
{   X509 *cert;
    char *line;
 
    cert = SSL_get_peer_certificate(ssl); /* get the server's certificate */
    if ( cert != NULL )
    {
        printf("Server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Subject: %s\n", line);
        free(line);       /* free the malloc'ed string */
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Issuer: %s\n", line);
        free(line);       /* free the malloc'ed string */
        X509_free(cert);     /* free the malloc'ed certificate copy */
    }
    else
        printf("Info: No client certificates configured.\n");
}
 



void writeMsg(void* args){
    SSL* sockfd = (SSL*) args;
    char buff[100]; 
    int n; 
    for (;;) { 
        bzero(buff, sizeof(buff)); 
        n = 0; 
        while ((buff[n++] = getchar()) != '\n') ; 
        if(SSL_write(sockfd, buff, sizeof(buff)) == -1) {
            exit(1); 
        }
        if ((strncmp(buff, "exit", 4)) == 0) { 
            printf("Exit...\n"); 
            exit(1);
        } 
        
    } 

}


void readMsg(void* args){
    SSL* ssl = (SSL*) args;
    char buff[100];
    int n;
    for(;;){
        bzero(buff,sizeof(buff));
        n = SSL_read(ssl,buff,sizeof(buff));
        buff[n] = '\0';
        printf("Friend : %s",buff);
    //LoadCertificates(ctx, "testCSE_chain.pem", "testcse.key"); /* load certs */
    }

}


int main(int argc, char *argv[])
{   SSL_CTX *ctx;
    int server;
    SSL *ssl;
    char buf[100];
    //char acClientRequest[1024] ={0};
    int bytes;
    char *hostname, *portnum;
 
    if ( argc !=4 )
    {
        printf("usage: %s <portnum> <client.pem> <client.key>\n", argv[0]);
        exit(1);
    }
    SSL_library_init();
    portnum=argv[1];
    
    ctx = InitCTX();
    server = OpenConnection(hostname, atoi(portnum));

    SSL_CTX_load_verify_locations(ctx, "root.pem", "/home/mayank/Documents/Sem6/Networks/NetSec/CA");
    LoadCertificates(ctx, argv[2], argv[3]); /* load certs */

    ssl = SSL_new(ctx);      /* create new SSL connection state */
    SSL_set_fd(ssl, server);    /* attach the socket descriptor */
    if ( SSL_connect(ssl) == FAIL )   /* perform the connection */
        ERR_print_errors_fp(stderr);
    else
    {  

        printf("\n\nConnected with %s encryption\n", SSL_get_cipher(ssl));
        ShowCerts(ssl);        /* get any certs */
        pthread_t thread_id[2];

        pthread_create(&thread_id[0], NULL,writeMsg,(void *)ssl); 
        pthread_create(&thread_id[1],NULL,readMsg,(void*)ssl);
        for(int i =0;i<2;i++) pthread_join(thread_id[i], NULL);

        SSL_free(ssl);        /* release connection state */
    }
    close(server);         /* close socket */
    SSL_CTX_free(ctx);        /* release context */
    return 0;
}
