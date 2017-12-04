/**
 * CS3510 :: OS 1
 * Programming assignment 2
 * Author: Supreet Singh CS16BTECH11038
 * Desc: Uses multi-processes to find prime numbers < Given input number.
 * Each Process writes to a memory only it has write access to.
 * Writes the output to a file (Primes-proc.txt).
 */

#include <iostream>
#include <fstream>
#include <cstdlib>          /* For atoi */
#include <fcntl.h>          /* For O_* constants */
#include <sys/stat.h>       /* For mode constants */
#include <sys/mman.h>       /* For shm_open */
#include <sys/wait.h>       /* For wait */
#include <unistd.h>         /* The next two are needed for ftruncate */
#include <sys/types.h>
#include <chrono>           /* For clock */

using namespace std;

int main(int argc, char **argv){
    int n, k;
    
    string nm="SM"; //name template
    
    cout<<"Enter n: ";
    cin>>n;
    cout<<"Enter k (2<=k<=32): ";
    cin>>k;
    const int SIZE = sizeof(int) * ((n-1)/k+1); //This much shared memory can be held at max!
    const char *name[k];
    for(int i=0; i<k; i++){ //set shared memory names
        name[i]=(nm+to_string(i)).c_str(); //-std=c++11         
    } //for
    
    pid_t pids[k]; //to store pids of children
    /* Start measuring time now */
    std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();

    for(int i=0; i<k; i++){ //Fork loop! creates k child processes.
        pids[i]=fork();
        if(pids[i]<0){ //error
            cerr<<"Error while forking child number "<<i<<endl;
        }
        else if(pids[i]==0){ //Child process. Exit in this else clause itself.
            int sh_fd = shm_open((nm+to_string(i)).c_str(), O_CREAT | O_RDWR, 0666);
            /* doesn't copy names[i] correctly. Possibly needs to be set to char *const name[] but then loop 
            assignment not possible. */
            if(sh_fd==-1){ //error handling
                cerr<<"Error in shm_open in child "<<i<<endl;
                exit(1);
            }
            if(ftruncate(sh_fd, SIZE)==-1){ //ftruncate
                cerr<<"Error in ftruncate in child "<<i<<endl;
                exit(1);
            } //ftr
            void *sh_ptr=NULL; //ptr to shared mem.

            sh_ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, sh_fd, 0);
            if(sh_ptr==MAP_FAILED){ //error handling
                cerr<<"Error in mmap in child "<<i<<endl;
                exit(1);
            }

            /*Begin checking if each number in range is prime?*/
            int len=0;
            for(int j=i+2; j<n; j+=k){ // PROC 0 Starts with 2, all upto n-1! Check primes upto 
                //now check if j is prime!
                bool fl=true;
                for(int chk=2; chk*chk<=j;chk++) if(j%chk==0) {fl=false; break;}
                if(fl){ //if found prime.
                    len++;       
                    (static_cast<int *>(sh_ptr))[len]=j; //Number of primes stored.
                }
            } //outer for
            (static_cast<int *>(sh_ptr))[0]=len;
            munmap(sh_ptr, SIZE); //unmap this

            //cout<<"child "<<i<<" exits"<<endl;
            exit(0); //Make the child exit here itself to prevent fork chain!
        } //Child process
    } //fork for

    int exs; //for error handling?
    //Wait for all child processes to finish. Wait returns <=0 if no child
    while(wait(&exs)>0); /*if(WEXITSTATUS(exs)==-1) return 1;
    else continue;// keep waiting otherwise
    */
    /* Now print the contents of all shared memories, one by one */
    
    int sh_fd;
    ofstream mf;
    mf.open("Primes-proc.txt");
    for(int i=0; i<k; i++){ //for loop to print shared memory

        name[i]=(nm+to_string(i)).c_str();
        sh_fd = shm_open(name[i], O_RDONLY, 0666);
        if(sh_fd==-1){ //error handling
            cerr<<"Error in shm_open in parent "<<endl;
            exit(1);
        }
        //no need of ftruncate here, we are only reading!

        void *sh_ptr=NULL; //ptr to shared mem.

        sh_ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, sh_fd, 0);
        if(sh_ptr==MAP_FAILED){ //error handling
            cerr<<"Error in mmap in parent"<<endl;
            exit(1);
        }

        int len = (static_cast<int *>(sh_ptr))[0];
        for(int j=1; j<=len; j++) mf<<(static_cast<int *>(sh_ptr))[j]<<endl;
        munmap(sh_ptr, SIZE);
        shm_unlink(name[i]);
    } //for
    mf.close(); //close the file

    /* Time measurement stops here*/
    std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double> time_taken = end - start;
    cout<<"Time taken (in seconds): "<<time_taken.count()<<endl;
    return 0;
} //main ends