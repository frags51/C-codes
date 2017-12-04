#include <stdio.h>
void swapAt(int *arr, int i, int j){
    int x = arr[i];
    arr[i] = arr[j];
    arr[j] = x;
    /*
    arr[i] = arr[i] ^ arr[j];
    arr[j] = arr[i] ^ arr[j];
    arr[i] = arr[i] ^ arr[j];
    //Why is this giving an error?
    
    */
}
int partition(int *arr, int lo, int hi){ //first element is pivot
    int i=lo, j=hi;
    int pivot = arr[lo];
    while(1){
        while(i < hi && arr[i] <= pivot) i++;
        while(arr[j] > pivot) j--;
        if(i < j) swapAt(arr, i, j);
        else break;
    }
    swapAt(arr, lo, j);
    return j;
}

void qs(int *arr, int lo, int hi){
    if(lo < hi){
        int j = partition(arr, lo, hi);
        qs(arr, lo, j-1);
        qs(arr, j+1, hi);
    } //if 
} //qs

int main(void){
    int n;
    scanf("%d", &n);
    int i = 0;    
    int arr[n];
       
    while(i<n) scanf("%d", &arr[i++]);

    qs(arr, 0, n-1);
    i = 0;
    while(i<n) printf("%d", arr[i++]);
   
    return 0;
}