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
#include <cstdlib>
using namespace std;

// Global Variables, Needed by each thread
int n, k; 
double l1, l2;
double wait_time = 0.0, worst_time = 0.0;
FILE *fp = fopen("TAS-log.txt", "w");
FILE *fp2 = fopen("TTAS-log.txt", "w");
FILE *fp3 = fopen("backoff-log.txt", "w");

std::default_random_engine e1;
std::default_random_engine e2; 
std::exponential_distribution<double> csRand; // Time in seconds 
std::exponential_distribution<double> rmRand;


class TASLock
{
private:
    std::atomic<bool> lck;
public:
    TASLock(){
        lck = false;
    }
    ~TASLock()=default;
    void lock(){
        while(lck.exchange(true));
    }
    void unlock(){
        lck.store(false);
    }
};

class TTASLock
{
    std::atomic<bool> lck;
public:
    TTASLock(){
        lck = false;
    }
    ~TTASLock() = default;
    void lock(){
        while(true){
            while(lck.load());
            if(!lck.exchange(true)) return;
        }        
    }
    void unlock(){
        lck.store(false);
    }
};

class Backoff
{
public:
    int minDelay, maxDelay;
    int limit;
    Backoff(int min, int max){
        minDelay = min;
        maxDelay = max;
        limit = minDelay;
        srand(time(NULL));
    }
    ~Backoff()=default;
    void backoff(){
        int delay = (rand())%limit;
        limit = maxDelay < 2*limit? maxDelay : 2*limit;
        std::this_thread::sleep_for(std::chrono::duration<double, std::micro>(delay));

    }
    
};

class BackoffLock
{
    std::atomic<bool> lck;
    const int minDelay = 1;
    const int maxDelay = 32;
public:
    BackoffLock(){
        lck = false;
    }
    ~BackoffLock()=default;
    void lock(){
        Backoff back(minDelay, maxDelay);
        while(true){
            while(lck.load());
            if(!lck.exchange(true)) return;
            else back.backoff();
        }  
    }

    void unlock(){
        lck = false;
    }
    
};

TASLock* tasLock = nullptr;
TTASLock* ttasLock = nullptr;
BackoffLock* backoffLock = nullptr;

void *testCS1(void *g){ // bakery Lock
    long long id=(long long) (g); // Tid: 1, 2 etc
    char buf[40]; // To store time
    for(int i=0; i<k; i++){ // Exec 'k' Times
        auto _reqTime = chrono::system_clock::now(); //the timepoint
        time_t reqTime = std::chrono::system_clock::to_time_t(_reqTime);
        struct tm* tinfo= localtime(&reqTime);
        strftime (buf, sizeof (buf), "%H:%M:%S", tinfo);
        fprintf(fp, "%dth CS request by Thread %lld at %s\n", i, id, buf);
        
        tasLock->lock();
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
        worst_time = worst_time > _wt.count() ? worst_time : _wt.count();
        tinfo= localtime(&exitTime);
        strftime (buf, sizeof (buf), "%H:%M:%S", tinfo);
        fprintf(fp, "%dth CS Exit by Thread %lld at %s\n", i, id, buf);

        tasLock->unlock();

        // REMAINDER SECTION
        std::this_thread::sleep_for(std::chrono::duration<double>(rmRand(e2)));
    } // Execing 'k' times for
    pthread_exit(0);
}

void *testCS2(void *g){ // filter Lock
    long long id=(long long) (g); // Tid: 1, 2 etc
    char buf[40]; // To store time
    for(int i=0; i<k; i++){ // Exec 'k' Times
        auto _reqTime = chrono::system_clock::now(); //the timepoint
        time_t reqTime = std::chrono::system_clock::to_time_t(_reqTime);
        struct tm* tinfo= localtime(&reqTime);
        strftime (buf, sizeof (buf), "%H:%M:%S", tinfo);
        fprintf(fp2, "%dth CS request by Thread %lld at %s\n", i, id, buf);
        
        ttasLock->lock();
        // Enter CS
        auto _enterTime = chrono::system_clock::now(); //the timepoint
        auto enterTime = std::chrono::system_clock::to_time_t(_enterTime);
        tinfo= localtime(&enterTime);
        strftime (buf, sizeof (buf), "%H:%M:%S", tinfo);
        fprintf(fp2, "%dth CS entry by Thread %lld at %s\n", i, id, buf);

        std::this_thread::sleep_for(std::chrono::duration<double>(csRand(e1)));

        // Exit CS

        auto _exitTime = chrono::system_clock::now(); //the timepoint
        auto exitTime = std::chrono::system_clock::to_time_t(_exitTime);
        chrono::duration<double> _wt= _enterTime - _reqTime;
        wait_time+= _wt.count(); // Setting Wait Time
        worst_time = worst_time > _wt.count() ? worst_time : _wt.count();

        tinfo= localtime(&exitTime);
        strftime (buf, sizeof (buf), "%H:%M:%S", tinfo);
        fprintf(fp2, "%dth CS Exit by Thread %lld at %s\n", i, id, buf);
        
        ttasLock->unlock();
        // REMAINDER SECTION

        std::this_thread::sleep_for(std::chrono::duration<double>(rmRand(e2)));
    } // Execing 'k' times for
    pthread_exit(0);
}

void *testCS3(void *g){
    long long id=(long long) (g); // Tid: 1, 2 etc
    char buf[40]; // To store time
    for(int i=0; i<k; i++){ // Exec 'k' Times
        auto _reqTime = chrono::system_clock::now(); //the timepoint
        time_t reqTime = std::chrono::system_clock::to_time_t(_reqTime);
        struct tm* tinfo= localtime(&reqTime);
        strftime (buf, sizeof (buf), "%H:%M:%S", tinfo);
        fprintf(fp3, "%dth CS request by Thread %lld at %s\n", i, id, buf);
        
        backoffLock->lock();
        // Enter CS
        auto _enterTime = chrono::system_clock::now(); //the timepoint
        auto enterTime = std::chrono::system_clock::to_time_t(_enterTime);
        tinfo= localtime(&enterTime);
        strftime (buf, sizeof (buf), "%H:%M:%S", tinfo);
        fprintf(fp3, "%dth CS entry by Thread %lld at %s\n", i, id, buf);

        std::this_thread::sleep_for(std::chrono::duration<double>(csRand(e1)));

        // Exit CS

        auto _exitTime = chrono::system_clock::now(); //the timepoint
        auto exitTime = std::chrono::system_clock::to_time_t(_exitTime);
        chrono::duration<double> _wt= _enterTime - _reqTime;
        wait_time+= _wt.count(); // Setting Wait Time
        worst_time = worst_time > _wt.count() ? worst_time : _wt.count();

        tinfo= localtime(&exitTime);
        strftime (buf, sizeof (buf), "%H:%M:%S", tinfo);
        fprintf(fp3, "%dth CS Exit by Thread %lld at %s\n", i, id, buf);
        
        backoffLock->unlock();
        // REMAINDER SECTION

        std::this_thread::sleep_for(std::chrono::duration<double>(rmRand(e2)));
    } // Execing 'k' times for
    pthread_exit(0);
}
/* Threads call this function */
int main(){
    ifstream inp{"inp-params.txt"}; 

    inp>>n>>k>>l1>>l2;

    // Actually these values are in milliseconds, but the program treats them as seconds
    l1/=1000;
    l2 /= 1000;
    e1.seed(std::chrono::system_clock::now().time_since_epoch().count());
    e2.seed(std::chrono::system_clock::now().time_since_epoch().count());
    csRand = std::exponential_distribution<double>(1/l1);
    rmRand = std::exponential_distribution<double>(1/l2);

    tasLock = new TASLock();
    ttasLock = new TTASLock();
    backoffLock = new BackoffLock();
    
    wait_time = 0.0;
    worst_time = 0.0;

    pthread_t tids[n]; // thread ID
    pthread_attr_t attrs[n]; // to set def attribs

    // For Filter Lock
    //printf("%d, %d, %lld, %lld\n", n, k, csSeed, remSeed);
    for(int _i=0; _i<n; _i++){ // Creating n threads.

        pthread_attr_init(&attrs[_i]); // to set default attribs for thread creation.
        if(pthread_create(&tids[_i], &attrs[_i], testCS1, (void *)_i)!=0){ // in case of an error
            cerr<<"Failed to create thread id: "<<_i<<endl;
        } //error checking
    } // for

    for (int j = 0; j < n; ++j) { // Wait for all threads to exit
        pthread_join(tids[j], NULL);
    }

    wait_time=wait_time/(n*k);
    cout<<"TAS Lock avg: "<< wait_time<<endl;
    cout<<"TAS Lock worst: "<< worst_time<<endl;
    cout<<"------------------\n";
    wait_time = 0.0;
    worst_time = 0.0;

    // For TTAS Lock
    //printf("%d, %d, %lld, %lld\n", n, k, csSeed, remSeed);
    for(int _i=0; _i<n; _i++){ // Creating n threads.

        pthread_attr_init(&attrs[_i]); // to set default attribs for thread creation.
        if(pthread_create(&tids[_i], &attrs[_i], testCS2, (void *)_i)!=0){ // in case of an error
            cerr<<"Failed to create thread id: "<<_i<<endl;
        } //error checking
    } // for

    for (int j = 0; j < n; ++j) { // Wait for all threads to exit
        pthread_join(tids[j], NULL);
    }

    wait_time=wait_time/(n*k);
    cout<<"TTAS Lock avg: "<< wait_time<<endl;
    cout<<"TTAS Lock worst: "<< worst_time<<endl;
    cout<<"------------------\n";

//////////////////////////

    wait_time = 0.0;
    worst_time = 0.0;

    // For backoff Lock
    //printf("%d, %d, %lld, %lld\n", n, k, csSeed, remSeed);
    for(int _i=0; _i<n; _i++){ // Creating n threads.

        pthread_attr_init(&attrs[_i]); // to set default attribs for thread creation.
        if(pthread_create(&tids[_i], &attrs[_i], testCS3, (void *)_i)!=0){ // in case of an error
            cerr<<"Failed to create thread id: "<<_i<<endl;
        } //error checking
    } // for

    for (int j = 0; j < n; ++j) { // Wait for all threads to exit
        pthread_join(tids[j], NULL);
    }

    wait_time=wait_time/(n*k);
    cout<<"backoff Lock avg: "<< wait_time<<endl;
    cout<<"backoff Lock worst: "<< worst_time<<endl;
    cout<<"------------------\n";

    return 0;
} // main
