#ifndef _SV_H
#define _SV_H

// usae: .file port
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/time.h> 
#include <netinet/in.h>
#include <sys/types.h> 
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <cmath>
#include <vector>
#include <mutex>

#define MAX_ARGS 40
const int N = 10; // max number of threads to create. 

#define SQRT 0
#define ADD 1
#define DIV 2
#define ANS 3

typedef struct _msg{
    int id; // what type of function etc
    int argsize;
    float args[MAX_ARGS];
} msg_struct;


#endif