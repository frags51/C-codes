/**
 * CS3510 :: OS 1
 * Programming assignment 2
 * Author: Supreet Singh CS16BTECH11038
 * Desc: Uses multi-threading to find prime numbers < Given input number.
 * Each Thread writes to its own array (which is shared)
 * Writes the output to a file (Primes-th.txt).
 */

#include <iostream>     /* Writing to stdout, stderr */
#include <fstream>      /* File writing */
#include <pthread.h>    /* POSIX pthread library */
#include <chrono>       /* For clock */

using namespace std;

/* Threads call this function */
void *prime(void *);

/* Global variables, needed by each thread */
int n, k;
int **sh_arr; // Pointer to shared array of arrays

int main(int argc, char *argv[]){
    /* Get input from the user! */
    cout<<"Enter n: ";
    cin>>n;
    cout<<"Enter k (2<=k<=32): ";
    cin>>k;
    const int SIZE = sizeof(int) * ((n-1)/k+1); //This much shared memory can be held at max! +1 to store length of arr.

    /* Create 'k' arrays, each thread will write to its own array.
     * This should be done on heap as only then do all threads share it!
     */

    sh_arr = new int*[k];
    for (int i = 0; i < k; ++i) {
        sh_arr[i] = new int[SIZE];
    }

    /* Array to store tids.. This are only needed by the parent */
    pthread_t tids[k];
    pthread_attr_t attrs[k]; // to set def attribs

    /* Start time measurement */
    std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();

    /* Now create k threads and perform the required computations */
    for(int i=0; i<k; i++){
        pthread_attr_init(&attrs[i]); // to set default attribs for thread creation.

        /* Need to pass the loop counter to thread, but can only be passed by reference, so create in heap */
        int *arg = new int(i);

        if(pthread_create(&tids[i], &attrs[i], prime, arg)!=0){ // in case of an error
            cerr<<"Failed to create thread id: "<<i<<endl;
        } //error checking
    } //for loop creating threads

    /* Wait for all threads to complete. */
    for (int j = 0; j < k; ++j) {
        pthread_join(tids[j], NULL);
    }

    /* Print these prime values onto a file now. */
    ofstream mf;
    mf.open("Primes-th.txt");
    for (int i = 0; i < k; ++i) {
        for (int j = 1; j <= sh_arr[i][0]; ++j) {
            mf<<sh_arr[i][j]<<endl;
        } // inner for
    } //outer for
    mf.close();

    /* Freeing heap */
    for (int i = 0; i < k; ++i) {
        delete sh_arr[i];
    }
    delete sh_arr;

    /* Time measurement stops here*/
    std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_taken = end - start;
    cout<<"Time taken (in seconds): "<<time_taken.count()<<endl;

    return 0;
} // main ends

void *prime(void *arg){ // Function to check the primes! > This needs the id (2-32, not the actual tid) and k

    int len=0; // This stores the number of prime numbers in the i'th array
    int i = static_cast<int *>(arg)[0]; // ==> *((int *)arg)

    for(int j=i+2; j<n; j+=k){ // PROC 0 Starts with 2, all upto n-1! Check primes upto
        //now check if j is prime!
        bool fl=true;
        for(int chk=2; chk*chk<=j;chk++) if(j%chk==0) {fl=false; break;}
        if(fl){ //if found prime.
            len++;
            sh_arr[i][len]=j; //Number of primes stored.
        }
    } //outer for

    sh_arr[i][0]=len;

    /* Termination conditions */
    delete static_cast<int *>(arg); // no need of this arg now! delete of void * is not defined though.
    pthread_exit(0);
} //prime ends