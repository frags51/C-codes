#include <stdio.h>
#include "heapsort.h"
#include <stdlib.h>
#include <time.h>
int main(void){
    uint64_t i; 
    uint64_t n=1024L*1024*1024;
   time_t t;
   int g[n/1024];
   /* Intializes random number generator */
   srand((unsigned) time(&t));
   FILE *fp = fopen(PATH, "w+b");
   int pos=0;
   for(i=0; i<n; i++) {
       g[pos++]=rand();
        if(pos==n/1024) {fwrite(&g, sizeof(uint32_t), n/1024, fp); pos=0;}
        
    }
   return 0;
}