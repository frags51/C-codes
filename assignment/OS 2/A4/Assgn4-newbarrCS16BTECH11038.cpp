/**
 * CS16BTECH11038
 * OS2 Assignment 3
 * Fair solution to reader/writer problem
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
int n, k;
int csSeed, remSeed;
//vector<double> wait_time;
double * wait_time;
// The random number generators.
std::default_random_engine e1; // For C S
std::default_random_engine e2; // For REM
std::uniform_int_distribution<int> csRand(1000, 5000); // Time in microseconds 
std::uniform_int_distribution<int> rmRand(1000, 4000);
// The log file for output
FILE *fp = fopen("new-barr-log.txt", "w");

class NewBarrier{
public:
    int n; // Number of threads
    int count; // count of threads currently in barrier.
    sem_t barr1, barr2, mtx;
    void init(int n){ // Initialize this barreir
        this->n=n;
        count=0;
        sem_init(&barr1, 0,0);
        sem_init(&barr2, 0, 1);
        sem_init(&mtx, 0, 1);
    }
    void barrierPoint(void){ // Wait for all n threads to reach the barrier.
        sem_wait(&mtx); // Lock mutex, increment count etc.
        count++;
        if(count==n){ // The nth thread reached the barrier
            sem_wait(&barr2); // Lock barr2
            sem_post(&barr1); // Signal barr1, allow one thread to cross the barrier.
        }
        sem_post(&mtx); // Unlock mutex

        sem_wait(&barr1); // Wait for barr1, until nth thread signals
        sem_post(&barr1); // Signal another waiting thread.
        // After all n threads pass, barr1 is 1
        // Now wait for all n threads to get past the barrier.
        sem_wait(&mtx);
        count--;
        if(count==0){ // All threads have crossed the barrier
            sem_wait(&barr1); // Lock the first barr (Similar to init'ing the semaphore again)
            sem_post(&barr2); // Signal barr2, allowing all n threads to go through now.
        }
        sem_post(&mtx);
        
        sem_wait(&barr2);
        sem_post(&barr2); // Similar to what was done in barr1.
    } // barrierPoint
}; // class NewBarrier
 
NewBarrier _barrier; // A global variable 

// Writer Function
void *bthread(void *g){
    double wt{0};
    long long id = (long long) g; // The pointer is just the id:1, 2 etc. Not an actual address.
    char buf[40]; // To store time
    auto t1 = chrono::system_clock::now();
    for(int i=0; i<k ;i++)
    {
        // Sleep doing some work
        auto _reqTime = chrono::system_clock::now(); //the timepoint
        time_t reqTime = std::chrono::system_clock::to_time_t(_reqTime);
        struct tm* tinfo= localtime(&reqTime);
        strftime (buf, sizeof (buf), "%H:%M:%S", tinfo);
        fprintf(fp, "Going to sleep before %dth barrier invocation at %s by Thread %lld\n", i, buf,id);
        usleep(csRand(e1));

        auto b__reqTime = chrono::system_clock::now(); //the timepoint
        time_t AreqTime = std::chrono::system_clock::to_time_t(b__reqTime);
        struct tm* __tinfo= localtime(&AreqTime);
        strftime (buf, sizeof (buf), "%H:%M:%S", __tinfo);
        fprintf(fp, "Before %dth barrier invocation at %s by Thread %lld\n", i, buf,id);
        
        // Barrier Point!
        _barrier.barrierPoint();
        // Synchronised!
        auto _enterTime = chrono::system_clock::now(); //the timepoint
        auto enterTime = std::chrono::system_clock::to_time_t(_enterTime);
        tinfo= localtime(&enterTime);
        strftime (buf, sizeof (buf), "%H:%M:%S", tinfo);
        fprintf(fp, "After %dth barrier invocation at %s by Thread %lld\n", i, buf,id);

        
        // Do random work again.

        auto _exitTime = chrono::system_clock::now(); //the timepoint
        auto exitTime = std::chrono::system_clock::to_time_t(_exitTime);
        //chrono::duration<double> _wt= _enterTime - b__reqTime;
        //wt+= _wt.count(); // Setting Wait Time
        tinfo= localtime(&exitTime);
        strftime (buf, sizeof (buf), "%H:%M:%S", tinfo);
        fprintf(fp, "Going to sleep after %dth barrier invocation at %s by Thread %lld\n", i, buf,id);

        usleep(rmRand(e2));
    } // For loop to exec kw times
    auto t2 = chrono::system_clock::now();
    wt= ((chrono::duration<double>)(t2-t1)).count();
    
    wait_time[id]=wt; // Write the wait time.
    pthread_exit(0);
} // writer ends


int main(){
    // Get the input
    ifstream inp{"inp-params.txt"}; 
    //wait_time.reserve(n*sizeof(double)); // Make the vector of size n.
    //for(int f=0; f<n; f++) wait_time.push_back(0.0);
    inp>>n>>k>>csSeed>>remSeed;
    inp.close();
    wait_time = (double *) calloc(n, sizeof(double));
    e1.seed(csSeed);
    e2.seed(remSeed);
    // Initialize the semaphores now.
    _barrier.init(n);
    // Create Writer Threads Now.
    pthread_t tids_w[n]; // thread ID
    pthread_attr_t attrs_w[n]; // to set def attribs
    for(int _i=0; _i<n; _i++){ // Creating n threads.
        pthread_attr_init(&attrs_w[_i]); // to set default attribs for thread creation.
        if(pthread_create(&tids_w[_i], &attrs_w[_i], bthread, (void *)_i)!=0){ // in case of an error
            cerr<<"Failed to create Writer thread id: "<<_i<<endl;
        } //error checking
    } // for

    // Now wait for these threads to finish execution.
    for (int j = 0; j < n; ++j) { // Wait for all threads to exit
        pthread_join(tids_w[j], NULL);
    }
    

    ofstream avg{"Average_time.txt"};
    //avg<<wait_time<<endl;
    double twait{0};
    for(int j=0;j<n;j++){
        twait+=wait_time[j];
        avg<<wait_time[j]<<"\n";
    }
    avg<<(twait/n);
    fclose(fp);
    avg.close();
    free(wait_time);
    return 0;
} // main ends