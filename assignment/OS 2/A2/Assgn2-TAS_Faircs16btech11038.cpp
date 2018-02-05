/**
 * CS3523 :: OS 2
 * Programming assignment 2
 * Author: Supreet Singh CS16BTECH11038
 * Implementing Critical Section Using Test and Set (Bounded Time)
 */ 
#include <iostream>     /* Writing to stdout, stderr */
#include <fstream>      /* File writing */
#include <pthread.h>    /* POSIX pthread library */
#include <chrono>       /* For clock */
#include <random>       /* Random */
#include <unistd.h>     /* usleep() */
#include <atomic>
#include <ctime>
using namespace std;

int n, k; // Global Variables, Needed by each thread
long csSeed, remSeed;
double wait_time = 0.0;
FILE *fp = fopen("TAS_bounded-log.txt", "w");

bool *waiting;

std::default_random_engine e1;
std::default_random_engine e2; 
std::uniform_int_distribution<int> csRand(10, 50); // Time in microseconds 
std::uniform_int_distribution<int> rmRand(10, 30);

std::atomic_flag lock = ATOMIC_FLAG_INIT;

void *testCS(void *g){
    long long id=(long long) (g); // Tid: 1, 2 etc
    char buf[40]; // To store time
    bool key=false;
    for(int i=0; i<k; i++){ // Exec 'k' Times
        auto _reqTime = chrono::system_clock::now(); //the timepoint
        time_t reqTime = std::chrono::system_clock::to_time_t(_reqTime);
        struct tm* tinfo= localtime(&reqTime);
        strftime (buf, sizeof (buf), "%H:%M:%S", tinfo);
        fprintf(fp, "%dth CS request by Thread %lld at %s\n", i, id, buf);

        waiting[(int)id]=true;
        key=true;
        while(waiting[(int)id] && key) key=atomic_flag_test_and_set(&lock); //Busy Wait
        waiting[(int)id]=false;
        
        
        // Enter CS
        
        auto _enterTime = chrono::system_clock::now(); //the timepoint
        auto enterTime = std::chrono::system_clock::to_time_t(_enterTime);
        tinfo= localtime(&enterTime);
        strftime (buf, sizeof (buf), "%H:%M:%S", tinfo);
        fprintf(fp, "%dth CS entry by Thread %lld at %s\n", i, id, buf);

        usleep(csRand(e1)); // Simulate CS
        
        // Exit CS
        int j = ((int)id+1)%n;
        while ((j != (int)id) && !waiting[j]) j = (j + 1) % n;
                
        auto _exitTime = chrono::system_clock::now(); //the timepoint
        auto exitTime = std::chrono::system_clock::to_time_t(_exitTime);
        chrono::duration<double> _wt= _enterTime - _reqTime;
        wait_time+= _wt.count(); // Setting Wait Time
        tinfo= localtime(&enterTime);
        strftime (buf, sizeof (buf), "%H:%M:%S", tinfo);

        fprintf(fp, "%dth CS Exit by Thread %lld at %s\n", i, id, buf);
        // ** Unlock ** 
        if(j==(int)id) atomic_flag_clear(&lock);
        else waiting[j]=false;

        //Remainder Section
        usleep(rmRand(e2)); // Simulate Remainder Section

    } // Execing 'k' times for
    pthread_exit(0);
}

/* Threads call this function */
int main(){
    ifstream inp{"inp-params.txt"}; 

    inp>>n>>k>>csSeed>>remSeed;
    e1.seed(csSeed);
    e2.seed(remSeed);

    waiting = new bool[n];
    for(int r=0; r<n; r++) waiting[r]=false;

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
    ofstream avg{"Average_time_TAS_bounded.txt"};
    avg<<wait_time<<endl;
    delete[] waiting;
    return 0;
} // main
