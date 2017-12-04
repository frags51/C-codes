/*
CS16BTECH11038 --- SUPREET SINGH
CS1353 Assignment 1
Compare various quicksort algorithm variations.

INPUT: (int) Size of array
(int) Elements of the array.

Outputs To Console and three files the time taken for each algorithm to sort those arrays.
*/
#include <stdio.h>
#include <time.h>
#include <stdlib.h> //malloc
#include <math.h>
//
//#include <linux/random.h>
//#include <unistd.h>
//#include <sys/syscall.h>
//
void swapAt(int arr[], const int i, const int j){
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}//swapAt
int partition(int arr[], int lo, int hi, int pivot){
        int i = lo-1, j=lo;
        swapAt(arr, pivot, hi); //Shift pivot to last spot.
        int p_elem = arr[hi]; //Now pivot is at hi!
        for(; j<hi; j++) if(arr[j] < p_elem) {i++; swapAt(arr, i, j);}
        swapAt(arr,i+1, hi); //bring pivot to i+1th pos.(
        return i+1;
}

void qsort_l(int arr[], int lo, int hi){ //last element is pivot.
    if(lo<hi){
        int j = partition(arr, lo, hi, hi);
        qsort_l(arr, lo, j-1);
        qsort_l(arr, j+1, hi);
    } //check for 1 elemnt.
}
void qsort_m(int arr[], int lo, int hi){//Median of first, last and mid is pivot. 
    if(lo<hi){//check for 1 element left.
        int mid = (lo + hi) / 2;
        int j;
        if(arr[lo] <= arr[hi]){
            if(arr[mid] <= arr[lo]) j = partition(arr, lo, hi, lo);
            else{
                if(arr[mid]<arr[hi]) j = partition(arr, lo, hi, mid);
                else j = partition(arr, lo, hi, hi);
            }
        }
        else{
            if(arr[mid] <= arr[hi]) j = partition(arr, lo, hi, hi);
            else{
                if(arr[mid]<arr[lo]) j = partition(arr, lo, hi, mid);
                else j = partition(arr, lo, hi, lo);
            }

        }
        
        qsort_m(arr, lo, j-1);
        qsort_m(arr, j+1, hi);  
    } 
}
void qsort_r(int arr[], int lo, int hi){
    if(lo<hi){
        
        int pi = (rand() % (hi - lo + 1)) + lo; //random pivot selection.
        
        //pi = (rand() % (hi - lo + 1)) + lo;
        //printf("Pi: %d, lo:%d, hi: %d\n", pi,lo, hi);
        
        int j = partition(arr, lo, hi, pi);
        qsort_r(arr, lo, j-1);
        qsort_r(arr, j+1, hi);
    } //if
}//qsort_r

int main(void){
    struct timespec d; //to store time.
    clock_gettime(CLOCK_REALTIME, &d); //better than time(null) -> More resolution 
    srand(d.tv_nsec/1000 + d.tv_sec*1000000); //time in millis
    int n = 0;
    scanf("%d", &n); //Takes the size of array as an INPUT
    //int arr[n];
    int *arr = (int *) malloc(n*sizeof(int));
    int i = 0; //Counter variable.
    while(i < n) scanf("%d", &arr[i++]);
    //temp ;;
    int *temp1 = (int *) malloc(n*sizeof(int)); //Temp arrays to duplicate the arrays to pass in different algos
    int *temp2 = (int *) malloc(n*sizeof(int));
    //
    i = 0;
    while(i < n) {temp1[i] = arr[i]; i++;}
    i = 0;
    while(i < n) {temp2[i] = arr[i]; i++;}
    
    //clock_t st1 = clock();
    struct timespec s, e1, e2, e3;
    clock_gettime(CLOCK_MONOTONIC_RAW, &s);
   
    qsort_l(temp2, 0, n-1);
   
    clock_gettime(CLOCK_MONOTONIC_RAW, &e1);
   
    qsort_m(temp1, 0, n-1);
   
    clock_gettime(CLOCK_MONOTONIC_RAW, &e2);
   
    
    qsort_r(arr, 0, n-1);
    clock_gettime(CLOCK_MONOTONIC_RAW, &e3);
   
    i = 0;
    //To print these arrays uncomment: 
    //while(i < n) printf("%d ", arr[i++]);
    //i = 0;
    //printf("\n");
    //while(i < n) printf("%d ", temp1[i++]);
    //i = 0;
    //printf("\n"); 
    //  while(i < n) printf("%d ", temp2[i++]);
    //
    // printf("RDM num: %d\n", rand()%40);
    //

    //OUTPUT TO CONSOLE (Time in nanoseconds).
    printf("(l): %lf\n", (double)(e1.tv_sec - s.tv_sec)*1000 + (double)(e1.tv_nsec - s.tv_nsec)/1000000);
    printf("(m): %lf\n",(double)(e2.tv_sec - e1.tv_sec)*1000 + (double)(e2.tv_nsec - e1.tv_nsec)/1000000);
    printf("(r): %lf\n", (double)(e3.tv_sec - e2.tv_sec)*1000 + (double)(e3.tv_nsec - e2.tv_nsec)/1000000);
    //
    //Output to txt files (Time in milliseconds).
    FILE * f1, *f2, *f3;
    f1 = fopen("qsort_l.txt", "a");
    f2 = fopen("qsort_m.txt", "a");
    f3=fopen("qsort_r.txt", "a");
    fprintf(f1, "%lf\n", (double)(e1.tv_sec - s.tv_sec)*1000 + (double)(e1.tv_nsec - s.tv_nsec)/1000000);
    fprintf(f2, "%lf\n", (double)(e2.tv_sec - e1.tv_sec)*1000 + (double)(e2.tv_nsec - e1.tv_nsec)/1000000);
    fprintf(f3, "%lf\n", (double)(e3.tv_sec - e2.tv_sec)*1000 + (double)(e3.tv_nsec - e2.tv_nsec)/1000000);
    fclose(f1);
    fclose(f2);
    fclose(f3);
    //
    //free the dynamically allocated memory.
    free(temp1);
    free(arr);
    free(temp2);
    
    return 0;
} //main