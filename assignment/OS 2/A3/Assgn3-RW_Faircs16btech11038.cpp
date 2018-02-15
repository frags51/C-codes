#include <iostream>     /* Writing to stdout, stderr */
#include <fstream>      /* File writing */
#include <pthread.h>    /* POSIX pthread library */
#include <chrono>       /* For clock */
#include <random>       /* Random */
#include <unistd.h>     /* usleep() */
#include <atomic>
#include <ctime>
#include <mutex>
#include <semaphore.h>  /* For Semaphores */

using namespace std;

// Global Vars, to be shared among all threads
int nw, nr, kw, kr;
int csSeed, remSeed;
double wait_time = 0.0;
// The random number generators.
std::default_random_engine e1; // For C S
std::default_random_engine e2; // For REM
std::uniform_int_distribution<int> csRand(10, 50); // Time in microseconds 
std::uniform_int_distribution<int> rmRand(10, 40);
// The log file for output
FILE *fp = fopen("FairRW-log.txt", "w");
// The semaphores - Initialized in main
sem_t rw_mutex;
sem_t mtx;
sem_t fair;
int read_count = 0;

// Writer Function
void *writer(void *g){
    long long id = (long long) g; // The pointer is just the id:1, 2 etc. Not an actual address.
    char buf[40]; // To store time
    for(int i=0; i<kw ;i++)
    {
        // Request CS
        auto _reqTime = chrono::system_clock::now(); //the timepoint
        time_t reqTime = std::chrono::system_clock::to_time_t(_reqTime);
        struct tm* tinfo= localtime(&reqTime);
        strftime (buf, sizeof (buf), "%H:%M:%S", tinfo);
        fprintf(fp, "%dth CS request by Writer Thread %lld at %s\n", i, id, buf);
        sem_wait(&fair);
        sem_wait(&rw_mutex); 
        
        // Enter CS // Do writing!
        auto _enterTime = chrono::system_clock::now(); //the timepoint
        auto enterTime = std::chrono::system_clock::to_time_t(_enterTime);
        tinfo= localtime(&enterTime);
        strftime (buf, sizeof (buf), "%H:%M:%S", tinfo);
        fprintf(fp, "%dth CS entry by Writer Thread %lld at %s\n", i, id, buf);

        usleep(csRand(e1));
        
        // Exit CS

        auto _exitTime = chrono::system_clock::now(); //the timepoint
        auto exitTime = std::chrono::system_clock::to_time_t(_exitTime);
        chrono::duration<double> _wt= _enterTime - _reqTime;
        wait_time+= _wt.count(); // Setting Wait Time
        tinfo= localtime(&exitTime);
        strftime (buf, sizeof (buf), "%H:%M:%S", tinfo);
        fprintf(fp, "%dth CS Exit by Writer Thread %lld at %s\n", i, id, buf);

        sem_post(&rw_mutex); // signal rw_mutex
        sem_post(&fair);
        usleep(rmRand(e2));
    } // For loop to exec kw times
    pthread_exit(0);
} // writer ends

void *reader(void *g){
    long long id = (long long) g; // The pointer is just the id:1, 2 etc. Not an actual address.
    char buf[40]; // To store time
    for(int i=0; i<kr ;i++)
    {
        auto _reqTime = chrono::system_clock::now(); //the timepoint
        time_t reqTime = std::chrono::system_clock::to_time_t(_reqTime);
        struct tm* tinfo= localtime(&reqTime);
        strftime (buf, sizeof (buf), "%H:%M:%S", tinfo);
        fprintf(fp, "%dth CS request by Reader Thread %lld at %s\n", i, id, buf);

        sem_wait(&fair);
        sem_wait(&mtx);
        read_count++;
        if (read_count == 1 ) {sem_wait(&rw_mutex);}
        sem_post(&mtx);
        sem_post(&fair);
        // Enter CS
        auto _enterTime = chrono::system_clock::now(); //the timepoint
        auto enterTime = std::chrono::system_clock::to_time_t(_enterTime);
        tinfo= localtime(&enterTime);
        strftime (buf, sizeof (buf), "%H:%M:%S", tinfo);
        fprintf(fp, "%dth CS entry by Reader Thread %lld at %s\n", i, id, buf);

        usleep(csRand(e1));

        // Exit CS

        auto _exitTime = chrono::system_clock::now(); //the timepoint
        auto exitTime = std::chrono::system_clock::to_time_t(_exitTime);
        chrono::duration<double> _wt= _enterTime - _reqTime;
        wait_time+= _wt.count(); // Setting Wait Time
        tinfo= localtime(&exitTime);
        strftime (buf, sizeof (buf), "%H:%M:%S", tinfo);
        fprintf(fp, "%dth CS Exit by Reader Thread %lld at %s\n", i, id, buf);

        sem_wait(&mtx);
        read_count--;
        if (read_count == 0) sem_post(&rw_mutex);
        sem_post(&mtx);
        // REMAINDER SECTION
        usleep(rmRand(e2));
    } // For loop to exec kw times
    pthread_exit(0);
} // reader ends


int main(){
    // Get the input
    ifstream inp{"inp-params.txt"}; 
    inp>>nw>>nr>>kw>>kr>>csSeed>>remSeed;
    inp.close();
    e1.seed(csSeed);
    e2.seed(remSeed);
    // Initialize the semaphores now (Both set to 1).
    sem_init(&rw_mutex,0,1);
    sem_init(&mtx ,0,1);
    sem_init(&fair, 0,1);

    // Create Writer Threads Now.
    pthread_t tids_w[nw]; // thread ID
    pthread_attr_t attrs_w[nw]; // to set def attribs
    for(int _i=0; _i<nw; _i++){ // Creating n threads.
        pthread_attr_init(&attrs_w[_i]); // to set default attribs for thread creation.
        if(pthread_create(&tids_w[_i], &attrs_w[_i], writer, (void *)_i)!=0){ // in case of an error
            cerr<<"Failed to create Writer thread id: "<<_i<<endl;
        } //error checking
    } // for

    // Create Reader Threads Now.
    pthread_t tids_r[nr]; // thread ID
    pthread_attr_t attrs_r[nr]; // to set def attribs
    for(int _i=0; _i<nr; _i++){ // Creating n threads.
        pthread_attr_init(&attrs_r[_i]); // to set default attribs for thread creation.
        if(pthread_create(&tids_r[_i], &attrs_r[_i], reader, (void *)_i)!=0){ // in case of an error
            cerr<<"Failed to create Reader thread id: "<<_i<<endl;
        } //error checking
    } // for

    // Now wait for these threads to finish execution.
    for (int j = 0; j < nw; ++j) { // Wait for all threads to exit
        pthread_join(tids_w[j], NULL);
    }
    for (int j = 0; j < nr; ++j) { // Wait for all threads to exit
        pthread_join(tids_r[j], NULL);
    }

    wait_time=wait_time/(nw*kw+nr*kr);
    ofstream avg{"Average_time_FairRW.txt"};
    avg<<wait_time<<endl;

    fclose(fp);
    avg.close();
    return 0;
} // main ends