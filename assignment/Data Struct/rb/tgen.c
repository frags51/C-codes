

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main(void){
    srand(time(NULL));
    int arr[1001];
    for(int i=0; i<1001; i++) arr[i] = 0;
    for(int i=0; i<10000; i++) {
        int a = rand()%1001;        
        if(arr[a] == 0) {printf("1 %d\n",a); arr[a]=1;}
    }
    printf("2\n5");
}
