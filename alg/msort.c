#include <stdio.h>
#include <stdlib.h>
int *arr2;
void merge(int *arr, int lo, int mid, int hi){ //merge two
    int i = lo, j = mid+1, ct=lo;
    while(i <= mid && j <= hi){
        if(arr[i] < arr[j]) {
            arr2[ct++] = arr[i++];
        } 
        else if(arr[i] > arr[j]) arr2[ct++] = arr[j++];
        else{
            arr2[ct++] = arr[i++];
            arr2[ct++] = arr[j++];
        }
    }
    while(i <= mid) arr2[ct++] = arr[i++];
    while(j<=hi) arr2[ct++] = arr[j++];
    for(int q = lo; q <=hi; q++) arr[q] = arr2[q]; //! ! ! ! ! ! ! ! !
}
void ms(int *arr, int lo, int hi){
    if(lo < hi){ //check for 1 element.
        int mid = (lo + hi) / 2;
        ms(arr, lo, mid);
        ms(arr, mid+1, hi);
        merge(arr, lo, mid, hi); //mid cuz we've used mid+1 in the merge program.
    }
}

int main(void){
    int n;
    scanf("%d", &n);
    int i = 0;    
    int arr[n];
    arr2 = (int *) malloc(n*sizeof(int));
    
    while(i<n) scanf("%d", &arr[i++]);

    ms(arr, 0, n-1);
    i = 0;
    while(i<n) printf("%d", arr[i++]);
    free(arr2);
    return 0;
}