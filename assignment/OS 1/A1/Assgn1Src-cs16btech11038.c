/**
 * Title: CS3510 :: OS 1
 * Sub: Programming Assignment 1
 * Author: CS16BTECH11038 Supreet Singh
 * Description: Computing Collatz Series in a child process created using fork()
 */

#include <stdio.h> 
#include <stdlib.h> /* For atoi */
#include <fcntl.h> /* For O_* constants */
#include <sys/stat.h> /* For mode constants */
#include <sys/mman.h> /* For shm_open */
#include <sys/wait.h> /* For wait */
#include <unistd.h> /* The next two are needed for ftruncate */
#include <sys/types.h>
#include <limits.h> /* For INT_MAX */

int main(int argc, char*argv[]){
    /* Variables common to both child and parent! */
    const char *name = "SM"; //name of the shared memory location
    const int SIZE = 1229*sizeof(long); //Number of Collatz numbers for n < INT_MAX is less than 987 (Internet)   

    /* ************** Processing INPUT NOW *********************** */
    
    int n;
    //scanf("%d", &n); // NOTE: THIS n is common to the child and the parent!
    if(argc!=2){ //argument error check
        fprintf(stderr, "Error! Provide only 1 Positive Integer cmd-line argument (< %d)\n", INT_MAX);
        return 1;
    } //argument error

    n = atoi(argv[1]); //returns 0 in case of invalid number
    if(n <= 0) { //Some error handling
        fprintf(stderr, "Error! Enter a positive number.. \n"); 
        return 1;
    }
    /* ***************** CREATE SHARED MEMORY ************************** */
    int sh_fd;
    sh_fd = shm_open(name, O_CREAT | O_RDWR, 0666); 
    /* 0666 Gives Rd/Wr but not execute permissions to all users */
     
    if(sh_fd==-1) { //in case shm_open fails
        fprintf(stderr, "SHM_OPEN FAILED! Exiting.. \n");
        return 1; //Exit from main
    } // shm_open

    if(ftruncate(sh_fd, SIZE)==-1){ // ftruncate failure
        fprintf(stderr, "ftruncate FAILED! Exiting.. \n");
        return 1;
    } // ftruncate
         
    void *sh_ptr = NULL; /* ptr to shared memory, set it to null initially. */
    /*  Note: This is copied to the child process as well! */
         
    sh_ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, sh_fd, 0);
    /* Map this memory for writing / reading (It is used by both parent and child). */
    if(sh_ptr == MAP_FAILED){
        printf("mmap FAILED! Exiting..\n");
        return 1;
    } // mmap
     

    pid_t pid = fork(); /* Fork a child process */

    if(pid==-1){ //Error handling
        fprintf(stderr, "fork FAILED! Exiting..\n");
        return 1;
    }
    else if(pid==0){ // code for the child process.
        //printf("Child: Got %d\n", n);

        long a = (long) n; //create copy of n.
        int len = 0; //The length of COLLATZ series
            
        while(a!=1L){
            len++;
            ((long *)sh_ptr)[len] = a; // Store the "long" coniguosly, starting from index 1
            if(a%2==0) a/=2; // Typecasting is needed as sh_ptr is of type void *
            else a = 3*a + 1;
        } //computing COLLATZ series.
        len++;
        ((long *)sh_ptr)[len] = 1; //need to store 1 as well
        ((long *)sh_ptr)[0] = (long) len; //store len as the first element.
        //Do not unlink here! Unlink in parent, since we need this file in parent
        munmap(sh_ptr, SIZE); //Unmap this memory mapping
    } //child
    else{ //parent process code
        int status;
        wait(&status); // Wait for child process to create and finish writing to the shared memory
        if(WEXITSTATUS(status)!=0){ //Some error handling
            fprintf(stderr, "Child terminated abnormally, so parent is exiting!\n");
            return 1;
        } //Wait status checking

        /* Print the Collatz Series */
        char fl=1; // Remove this message for automated checking of correctness
        if(fl) printf("The Collatz Series for %d is: ", n);
        int len = ((int *)sh_ptr)[0];
        
        for(int i=1; i<=len; i++) printf("%ld%c", ((long *)sh_ptr)[i], i==len?'\n':' ');
        munmap(sh_ptr, SIZE); //Unmap this memory mapping        
        shm_unlink(name); //Unlinking 
    } //parent

    return 0;
} //main ends