/**
 * CS3523 :: OS 2
 * Programming assignment 2
 * Author: Supreet Singh CS16BTECH11038
 * Implementing Critical Section Using Compare and Swap
 */ 
#include <iostream>     /* Writing to stdout, stderr */
#include <fstream>      /* File writing */
#include <pthread.h>    /* POSIX pthread library */
#include <chrono>       /* For clock */
#include <random>       /* Random */
#include <unistd.h>     /* usleep() */
#include <atomic>
#include <ctime>
#include <mutex>

using namespace std;

int n, k; // Global Variables, Needed by each thread
long csSeed, remSeed;
double wait_time = 0.0;
FILE *fp = fopen("CAS-log.txt", "w");

std::default_random_engine e1;
std::default_random_engine e2; 
std::uniform_int_distribution<int> csRand(10, 50); // Time in microseconds 
std::uniform_int_distribution<int> rmRand(10, 30);
//int _lock (0);
std::atomic<int > flock{0};
//int aexp2 {0}; // Cas function expexts pointer of this type
/*
mutex mtx;

int cas(int *value, int old, int new_v){
    mtx.lock();
    int temp = *value;
    if(*value==old) *value=new_v;
    mtx.unlock();
    return temp;
} 
int lk2 {0};
*/
void *testCS(void *g){
    long long id=(long long) (g); // Tid: 1, 2 etc
    char buf[40]; // To store time
    for(int i=0; i<k; i++){ // Exec 'k' Times
        auto _reqTime = chrono::system_clock::now(); //the timepoint
        time_t reqTime = std::chrono::system_clock::to_time_t(_reqTime);
        struct tm* tinfo= localtime(&reqTime);
        strftime (buf, sizeof (buf), "%H:%M:%S", tinfo);
        fprintf(fp, "%dth CS request by Thread %lld at %s\n", i, id, buf);

        while(1){
            int aexp=0;
            if(atomic_compare_exchange_strong(&flock, &aexp, 1)) break;
        } //Busy Wait
        //aexp2=0;
        
        // Enter CS
        auto _enterTime = chrono::system_clock::now(); //the timepoint
        auto enterTime = std::chrono::system_clock::to_time_t(_enterTime);
        tinfo= localtime(&enterTime);
        strftime (buf, sizeof (buf), "%H:%M:%S", tinfo);
        fprintf(fp, "%dth CS entry by Thread %lld at %s\n", i, id, buf);

        usleep(csRand(e1));

        // Exit CS

        auto _exitTime = chrono::system_clock::now(); //the timepoint
        auto exitTime = std::chrono::system_clock::to_time_t(_exitTime);
        chrono::duration<double> _wt= _enterTime - _reqTime;
        wait_time+= _wt.count(); // Setting Wait Time
        tinfo= localtime(&enterTime);
        strftime (buf, sizeof (buf), "%H:%M:%S", tinfo);
        fprintf(fp, "%dth CS Exit by Thread %lld at %s\n", i, id, buf);
        
        flock.store(0); // Set lock to false
        // REMAINDER SECTION
        usleep(rmRand(e2));
    } // Execing 'k' times for
    pthread_exit(0);
}

/* Threads call this function */
int main(){
    ifstream inp{"inp-params.txt"}; 

    inp>>n>>k>>csSeed>>remSeed;
    e1.seed(csSeed);
    e2.seed(remSeed);
    pthread_t tids[n]; // thread ID
    pthread_attr_t attrs[n]; // to set def attribs

    //printf("%d, %d, %lld, %lld\n", n, k, csSeed, remSeed);
    for(int _i=0; _i<n; _i++){ // Creating n threads.

        pthread_attr_init(&attrs[_i]); // to set default attribs for thread creation.
        if(pthread_create(&tids[_i], &attrs[_i], testCS, (void *)_i)!=0){ // in case of an error
            cerr<<"Failed to create thread id: "<<_i<<endl;
        } //error checking
    } // for

    for (int j = 0; j < n; ++j) { // Wait for all threads to exit
        pthread_join(tids[j], NULL);
    }

    wait_time=wait_time/(n*k);
    //cout<<wait_time<<endl;
    ofstream avg{"Average_time_CAS.txt"};
    avg<<wait_time<<endl;
    return 0;
} // main
