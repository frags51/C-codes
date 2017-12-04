#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/*
int bs(int arr[], int low, int high, int element)
{
	int middle;
	while(low <= high)
	{
		middle = (low + high) / 2;
		if(element > arr[middle])
			low = middle + 1;
		else if(element < arr[middle])
			high = middle - 1;
		else
			return middle;
	}
	return -1;
}
*/

//
int lower_bound(int arr[], int key, int low, int high)
{
    if (low > high)
        //return -1;
        return low;

    int mid = low + ((high - low) >> 1);
    //if (arr[mid] == key) return mid;

    //Attention here, we go left for lower_bound when meeting equal values
    if (arr[mid] >= key)
        return lower_bound(arr, key, low, mid - 1);
    else
        return lower_bound(arr, key, mid + 1, high);
}

int upper_bound(int arr[], int key, int low, int high)
{
    if (low > high)
        //return -1;
        return low;

    int mid = low + ((high - low) >> 1);
    //if (arr[mid] == key) return mid;

    //Attention here, we go right for upper_bound when meeting equal values
    if (arr[mid] > key)
        return upper_bound(arr, key, low, mid - 1);
    else
        return upper_bound(arr, key, mid + 1, high);
}
//
void Merge(int *A,int *L,int leftCount,int *R,int rightCount) {
	int i,j,k;

	i = 0; j = 0; k =0;

	while(i<leftCount && j< rightCount) {
		if(L[i]  < R[j]) A[k++] = L[i++];
		else A[k++] = R[j++];
	}
	while(i < leftCount) A[k++] = L[i++];
	while(j < rightCount) A[k++] = R[j++];
}

// Recursive function to sort an array of integers.
void MergeSort(int *A,int n) {
	int mid,i, *L, *R;
	if(n < 2) return;

	mid = n/2;

	L = (int*)malloc(mid*sizeof(int));
	R = (int*)malloc((n- mid)*sizeof(int));

	for(i = 0;i<mid;i++) L[i] = A[i]; // creating left subarray
	for(i = mid;i<n;i++) R[i-mid] = A[i]; // creating right subarray

	MergeSort(L,mid);  // sorting the left subarray
	MergeSort(R,n-mid);  // sorting the right subarray
	Merge(A,L,mid,R,n-mid);  // Merging L and R into A as sorted list.
        free(L);
        free(R);
}

int main(void){
  int x, n;
  scanf("%d %d", &n, &x);
  int arr[n];
  for(int i=0; i<n; i++) scanf("%d", &arr[i]);
  unsigned long int count = 0;
//  for(int i=0; i<=size-2; i++) for(int j=i+1; j<=size-i;j++) if((arr[i] ^ arr[j]) == x) {count++;}
//  printf("%d\n", count);
  MergeSort(arr,n);
  for(int j=0; j<n-1; j++){
    int r = lower_bound(arr, arr[j]^x, j+1, n-1);
    int s = upper_bound(arr, arr[j]^x, j+1, n-1);
    count += (s-r);
  }
//for(int l=0; l<n; l++) printf("\t%d", arr[l]);
  printf("%lu\n", count);
  return 0;
}
