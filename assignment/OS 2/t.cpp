#include <iostream>     /* Writing to stdout, stderr */
#include <fstream>      /* File writing */
#include <pthread.h>    /* POSIX pthread library */
#include <chrono>       /* For clock */
#include <random>       /* Random */
#include <unistd.h>     /* usleep() */
#include <atomic>
#include <ctime>
#include <mutex>
#include <semaphore.h>  /* For POSIX Semaphores */

sem_t b;

int main(){
    sem_init(&b, 0, 5);
    int a;
    sem_wait(&b);
    
    sem_getvalue(&b, &a);
    std::cout<<a<<std::endl;
}