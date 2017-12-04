/*
CS16BTECH11038 --- SUPREET SINGH
CS1353 Assignment 1

First prints the Size of the array to be generated to stdout. 
Then, generates RANDOM NUMBER Arrays of SIZE as defined, to stdout.
Output may then be piped to the sorting program.
*/
#define SIZE 250
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void){
    struct timespec s;
    clock_gettime(CLOCK_REALTIME, &s); 
    srand(s.tv_nsec/1000 + s.tv_sec*1000000); //--> for more resolution.
    printf("%d\n", SIZE);
   
    for(int i = 0; i< SIZE; i++) {
       
        printf("%d ", rand());
        }
    return 0;
}