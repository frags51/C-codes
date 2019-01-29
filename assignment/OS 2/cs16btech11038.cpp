/**
 * CS16BTECH11038
 * OS2 FInal Lab Exam
 * Hungry Tribe Problem
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
#include <semaphore.h>  /* For POSIX Semaphores */

using namespace std;

// Global Vars, to be shared among all threads
int K, M, n, a, b, l, m;
int csSeed, remSeed;
double wait_time = 0.0;
int nDone {0};
// The random number generators.
std::default_random_engine ea; // For a
 // For m

std::exponential_distribution<double> ad; // Time in microseconds 
std::exponential_distribution<double> bd; // Time in microseconds 
std::exponential_distribution<double> ld; // Time in microseconds 
std::exponential_distribution<double> md; // Time in microseconds 

// The log file for output
FILE *fp = fopen("log.txt", "w");
// The semaphores - Initialized in main
sem_t rw_mutex;
sem_t mtx;
sem_t fair;
sem_t nDLock; // To check atomically how many tribesman have finished.
int pot_count; // To store how many servings are there in the pot

short gg {0};

bool DEB {false}; // Fore debugging purposes

void *tribesman(void *g){
    long long id = (long long) g; // The pointer is just the id:1, 2 etc. Not an actual address.
    char buf[40]; // To store time
    for(int i=0; i<n ;i++)
    {
        auto _reqTime = chrono::system_clock::now(); //the timepoint
        time_t reqTime = std::chrono::system_clock::to_time_t(_reqTime);
        struct tm* tinfo= localtime(&reqTime);
        strftime (buf, sizeof (buf), "%H:%M:%S", tinfo);
        fprintf(fp, "%dth time the Thread %lld becomes hungry at %s\n", i, id, buf);

        sem_wait(&mtx); // mtx for community members
        sem_wait(&fair);
        if(DEB) std::cerr<<"mtx, fair\n";
        int _pc = pot_count;
        //sem_post(&fair);
        if (_pc == 0 ) {
            gg = -1;
            sem_post(&fair);
        if(DEB) std::cerr<<"mtx, rw_mutex\n";
            
            sem_wait(&rw_mutex);
        } // Wait for cook to fill!
        else sem_post(&fair);
        sem_wait(&fair);
        pot_count--; // getServingFromPot()
        sem_post(&fair);
        //sem_post(&mtx);
        //
        auto _enterTime = chrono::system_clock::now(); //the timepoint
        auto enterTime = std::chrono::system_clock::to_time_t(_enterTime);
        tinfo= localtime(&enterTime);
        strftime (buf, sizeof (buf), "%H:%M:%S", tinfo);
        fprintf(fp, "%dth time the Thread %lld eats at %s\n", i, id, buf);

        usleep(1000*ad(ea)); // eat()

        // Exit CS

        auto _exitTime = chrono::system_clock::now(); //the timepoint
        auto exitTime = std::chrono::system_clock::to_time_t(_exitTime);
        chrono::duration<double> _wt= _enterTime - _reqTime;
        wait_time+= _wt.count(); // Setting Wait Time
        tinfo= localtime(&exitTime);
        strftime (buf, sizeof (buf), "%H:%M:%S", tinfo);
        fprintf(fp, "%dth time the Thread %lld has eaten at %s\n", i, id, buf);

        //sem_wait(&mtx);
        sem_post(&mtx);
        // Do community-service()!
        usleep(1000*bd(ea));
    } // For loop to exec kw times
    sem_wait(&nDLock);
    nDone++;
    fprintf(fp, "Thread %lld has eaten n times, hence exits\n", id);
    sem_post(&nDLock);
    pthread_exit(0);
} // reader ends


int main(){
    // Get the input
    ifstream inp{"inp-params.txt"}; 
    inp>>K>>M>>n>>a>>b>>l>>m;
    pot_count = M;
    inp.close();
    ea.seed(4651556);

    ad = std::exponential_distribution<double> (10/a);
    bd = std::exponential_distribution<double> (10/b);
    ld = std::exponential_distribution<double> (10/l);
    md = std::exponential_distribution<double> (10/m);

    // Initialize the semaphores now (Both set to 1).
    sem_init(&rw_mutex,0,0); // Set to zero!
    sem_init(&mtx ,0,1);
    sem_init(&fair, 0,1);
    sem_init(&nDLock, 0,1);

    // Create Tribesman Threads Now.
    pthread_t tids_w[K]; // thread ID
    pthread_attr_t attrs_w[K]; // to set def attribs
    for(int _i=0; _i<K; _i++){ // Creating n threads.
        pthread_attr_init(&attrs_w[_i]); // to set default attribs for thread creation.
        if(pthread_create(&tids_w[_i], &attrs_w[_i], tribesman, (void *)_i)!=0){ // in case of an error
            cerr<<"Failed to create Writer thread id: "<<_i<<endl;
        } //error checking
    } // for

    bool flg {true};
    while(flg){ // DO THE ACTION OF COOK THREAD!
        char buf[40];
        sem_wait(&fair);
        if(pot_count==0) { // Cook : put servings in pot!
            // THE FUNCTION putServingsInPot()
            pot_count=M; 
            auto _enterTime = chrono::system_clock::now(); //the timepoint
            auto enterTime = std::chrono::system_clock::to_time_t(_enterTime);
            auto tinfo= localtime(&enterTime);
            strftime (buf, sizeof (buf), "%H:%M:%S", tinfo);
            fprintf(fp, "The cook fills the empty pot at %s\n", buf);

            usleep(1000*ld(ea));
            //int gg;
            //sem_getvalue(&rw_mutex, &gg);
            if(gg==-1) {gg=0;sem_post(&rw_mutex);}
        }
        sem_post(&fair);
        // cook-sleep()
        usleep(1000*md(ea));
        // Check if threads finished
        sem_wait(&nDLock);
        if(nDone==K) flg=false;
        sem_post(&nDLock);
    }

    // Now wait for these threads to finish execution.
    for (int j = 0; j < K; ++j) { // Wait for all threads to exit
        pthread_join(tids_w[j], NULL);
    }

    wait_time=wait_time/(K*n);
    ofstream avg{"Average_time.txt"};
    avg<<wait_time<<endl;

    fclose(fp);
    avg.close();
    return 0;
} // main ends