#include <stdio.h>
/*
This bs finds first occurance of the key.
Total FOUND: uplim - lolim
*/
int bsearch(int *arr, int lo,int hi, int key){
    while(lo <= hi){ // > 1 element left
        int mid = (lo + hi)/2;
        if(arr[mid] == key) return mid;
        else if(arr[mid] > key) hi = mid-1;
        else lo = mid + 1;
    }
    return -1;//if not found
}

int bs_lowlim(int *arr, int lo, int hi, int key){ //lowest found index, else index of next greatest element 
    while(lo <= hi){ // > 1 element left
        int mid = (lo + hi)/2;
        if(arr[mid] >= key) hi = mid-1;
        //else if(arr[mid] > key) hi = mid-1;
        else lo = mid + 1;
    }
    return lo;
}

int bs_uplim(int *arr, int lo, int hi, int key){ //highest found index + 1, else index of next greatest element (or ARR_SIZE)
    while(lo <= hi){ // > 1 element left
        int mid = (lo + hi)/2;
        if(arr[mid] <= key) lo = mid+1;
        //else if(arr[mid] > key) hi = mid-1;
        else hi = mid -1;
    }
    return lo;
}

int main(void){
    int n;
    scanf("%d", &n);
    int i = 0;    
    int arr[n];
    while(i<n) scanf("%d", &arr[i++]);
    int key;
    scanf("%d", &key);
    printf("Found @ %d \n", bsearch(arr, 0, n-1, key));
    printf("Found LOLIM @ %d \n", bs_lowlim(arr, 0, n-1, key));
    printf("Found HILIM @ %d \n", bs_uplim(arr, 0, n-1, key));
    return 0;
}