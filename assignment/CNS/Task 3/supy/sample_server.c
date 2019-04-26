#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <resolv.h>
#include "openssl/ssl.h"
#include "openssl/err.h"
#include <pthread.h>
 
#define FAIL    -1
SSL* c[2];

 
 
// Create the SSL socket and intialize the socket address structure
int OpenListener(int port)
{   int sd;
    struct sockaddr_in addr;
 
    sd = socket(PF_INET, SOCK_STREAM, 0);
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
    {
        perror("can't bind port");
        abort();
    }
    if ( listen(sd, 10) != 0 )
    {
        perror("Can't configure listening port");
        abort();
    }
    return sd;
}
 
int isRoot()
{
    if (getuid() != 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
 
}
SSL_CTX* InitServerCTX(void)
{   SSL_METHOD *method;
    SSL_CTX *ctx;
    int mode = SSL_VERIFY_PEER; 
    OpenSSL_add_all_algorithms();  /* load & register all cryptos, etc. */
    SSL_load_error_strings();   /* load all error messages */
    method = TLSv1_2_server_method();  /* create new server-method instance */
    ctx = SSL_CTX_new(method);   /* create new context from method */
    if ( ctx == NULL )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    SSL_CTX_set_verify(ctx, mode, NULL);
    SSL_CTX_load_verify_locations(ctx, "root.pem", "/media/supreet/Sup/C-codes/assignment/CNS/Task 3/supy");
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
 
    cert = SSL_get_peer_certificate(ssl); /* Get certificates (if available) */
    if ( cert != NULL )
    {
        printf("Server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Subject: %s\n", line);
        free(line);
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Issuer: %s\n", line);
        free(line);
        X509_free(cert);
    }
    else
        printf("No certificates.\n");
}
 
void Servlet(void* args) /* Serve the connection -- threadable */
{
    int i = (int) args;
    SSL* ssl = c[i];
    char buf[100] = {0};
    int sd, bytes;
 
    if ( SSL_accept(ssl) == FAIL )     /* do SSL-protocol accept */
        ERR_print_errors_fp(stderr);
    else
    {
        ShowCerts(ssl);        /* get any certificates */
        for(;;){
            bytes = SSL_read(ssl, buf, sizeof(buf)); /* get request */
            buf[bytes] = '\0';
            if(bytes == 0) break;
            if(strcmp(buf, "exit\n") == 0){
                printf("Hey\n");
            }
            printf("Client msg: \"%s\"\n", buf);
            if ( bytes > 0 )
            {

                SSL_write(c[1-i], buf, strlen(buf)); /* send reply */
                
            }

            else
            {
                ERR_print_errors_fp(stderr);
            }
            
        }
    }
    sd = SSL_get_fd(ssl);       /* get socket connection */
    SSL_free(ssl);         /* release SSL state */
    close(sd);          /* close connection */
}
 
int main(int argc, char *argv[])
{   SSL_CTX *ctx;
    int server;
    char *portnum;
 
    /*
    //Only root user have the permsion to run the server
    if(!isRoot())
    {
        printf("This program must be run as root/sudo user!!");
        exit(0);
    }
    */
    if ( argc < 2 )
    {
        printf("Usage: %s <portnum>\n", argv[0]);
        exit(0);
    }
 
 // Initialize the SSL library
    SSL_library_init();
 
    portnum = argv[1];
    ctx = InitServerCTX();        /* initialize SSL */
    LoadCertificates(ctx, "testCSE_chain.pem", "testcse.key"); /* load certs */
    server = OpenListener(atoi(portnum));    /* create server socket */
    pthread_t th[2];
    for(int i = 0;i<2;i++)
    {   struct sockaddr_in addr;
        socklen_t len = sizeof(addr);
 
        int client = accept(server, (struct sockaddr*)&addr, &len);  /* accept connection as usual */
        printf("Connection: %s:%d\n",inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
        c[i] = SSL_new(ctx);              /* get new SSL state with context */
        SSL_set_fd(c[i], client);      /* set connection socket to SSL state */
        pthread_create(th+i, NULL, Servlet, (void *) i);
        //Servlet(i);         /* service connection */
    }

    for(int i = 0;i<2;i++) pthread_join(th[i], NULL);
    close(server);          /* close server socket */
    SSL_CTX_free(ctx);         /* release context */
}
