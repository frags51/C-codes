/**
 * CS3523 :: PCP
 * Programming assignment 2
 * Author: Supreet Singh CS16BTECH11038
 * Lock - FIlter and Bakery
 */ 
#include <iostream>     /* Writing to stdout, stderr */
#include <fstream>      /* File writing */
#include <pthread.h>    /* POSIX pthread library */
#include <chrono>       /* For clock */
#include <random>       /* Random */
#include <unistd.h>     /* usleep() */
#include <atomic>
#include <ctime>
#include <thread>
using namespace std;

// Global Variables, Needed by each thread
int n, k; 
long l1, l2;
double wait_time = 0.0;
FILE *fp = fopen("Bakery-log.txt", "w");
FILE *fp2 = fopen("Filter-log.txt", "w");

std::default_random_engine e1;
std::default_random_engine e2; 
std::exponential_distribution<double> csRand; // Time in microseconds 
std::exponential_distribution<double> rmRand;


void *testCS(void *g){
    long long id=(long long) (g); // Tid: 1, 2 etc
    char buf[40]; // To store time
    for(int i=0; i<k; i++){ // Exec 'k' Times
        auto _reqTime = chrono::system_clock::now(); //the timepoint
        time_t reqTime = std::chrono::system_clock::to_time_t(_reqTime);
        struct tm* tinfo= localtime(&reqTime);
        strftime (buf, sizeof (buf), "%H:%M:%S", tinfo);
        fprintf(fp, "%dth CS request by Thread %lld at %s\n", i, id, buf);
        
        // Enter CS
        auto _enterTime = chrono::system_clock::now(); //the timepoint
        auto enterTime = std::chrono::system_clock::to_time_t(_enterTime);
        tinfo= localtime(&enterTime);
        strftime (buf, sizeof (buf), "%H:%M:%S", tinfo);
        fprintf(fp, "%dth CS entry by Thread %lld at %s\n", i, id, buf);

        std::this_thread::sleep_for(std::chrono::duration<double>(csRand(e1)));

        // Exit CS

        auto _exitTime = chrono::system_clock::now(); //the timepoint
        auto exitTime = std::chrono::system_clock::to_time_t(_exitTime);
        chrono::duration<double> _wt= _enterTime - _reqTime;
        wait_time+= _wt.count(); // Setting Wait Time
        tinfo= localtime(&enterTime);
        strftime (buf, sizeof (buf), "%H:%M:%S", tinfo);
        fprintf(fp, "%dth CS Exit by Thread %lld at %s\n", i, id, buf);
        // REMAINDER SECTION
        std::this_thread::sleep_for(std::chrono::duration<double>(rmRand(e2)));
    } // Execing 'k' times for
    pthread_exit(0);
}

/* Threads call this function */
int main(){
    ifstream inp{"inp-params.txt"}; 

    inp>>n>>k>>l1>>l2;
    e1.seed(std::chrono::system_clock::now().time_since_epoch().count());
    e2.seed(std::chrono::system_clock::now().time_since_epoch().count());
    csRand = std::exponential_distribution<double>(l1);
    rmRand = std::exponential_distribution<double>(l2);

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
    ofstream avg{"Average_time_TAS.txt"};
    avg<<wait_time<<endl;
    return 0;
} // main
