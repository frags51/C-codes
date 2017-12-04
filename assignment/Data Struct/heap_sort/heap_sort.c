//HEAP SORT USING MIN HEAP (DESCENDING!)
//CS16BTECH11038

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define HEAP_SIZE 1000

typedef struct _heap{
	int a[HEAP_SIZE];
	int ptr; //next position for element to be inserted
} heap;
//functions
int add(heap * h, int n){
  if(h->ptr >= HEAP_SIZE) return 0; //to make sure HEAP SIZE is maintained.
	h->a[h->ptr++] = n;
	int c = h->ptr - 1;
	int p = (c-1)/2; // parent  = (leaf-1/)2
	while(p>=0){//bubble up
		int temp = h->a[c];
		if(h->a[c] < h->a[p]){
			h->a[c] = h->a[p];
			h->a[p] = temp;
		}
		else break;
		temp = p; //SWAP
		c = p;
		p = (p-1)/2;	 
	}//while
	return 1; //added successfully
} //add.

int del(heap *h){
	if(h->ptr == 0) return 0;
	h->ptr--;
	if(h->ptr > 0){
		int c = h->ptr;
		int temp = h->a[0];
		h->a[0] = h->a[c];
		h->a[c] = temp;
		
		//sift down now:
		int now = 0, min_index=0;
		while(1){
			int leftChild = 2*now + 1;
			int rightChild = 2*now + 2;
			if(rightChild >= h->ptr){
				if(leftChild>=h->ptr) break; //reached the end!
				else {
					min_index = leftChild;
				} //leftchild exists but right doesn't
			}
			else{
				if(h->a[leftChild] <= h->a[rightChild]) min_index = leftChild;
				else min_index = rightChild;
			}
			if(h->a[now] > h->a[min_index]){ //!!!!!!!!!!!!!!!!!! THIS SWAP TAKES THE MINIMUM ELEMENT TO THE SUPPOSED END OF THE HEAP AT THIS TIME!
			  int temp = h->a[min_index]; // !!!!!!!!!!!!!! HENCE ARRAY IS SORTED IN DESCENDING ORDER ON DELETION TILL ALL THE ELEMENTS HAVE BEEN DELETED!
				h->a[min_index] = h->a[now];
				h->a[now] = temp;
				now = min_index;
			}
			else break;
		} //sift down loop!
	}
	return 1;
}// del
int isEmpty(heap *h){ //only to test if it's not empty.
  return !(h->ptr > 0);
}//peek
void print_h(heap *h){ //NOT in/pre/post tree traversal.
	for(int i=0; i<h->ptr; i++) printf("%d ", h->a[i]);
	printf("\n");
}
void heapify(heap *h){
  int par_ind = h->ptr - 1; //ptr is the next free point. par_ind set to the last parent in the heap.
  
  while(par_ind >= 0){//sift_down building the heap
    int min_ind = par_ind;
    while(1){ //actually sift_down happens here
      int leftChild = 2*par_ind + 1;
      int rightChild = 2*par_ind + 2;
      if(rightChild >= h->ptr && leftChild >= h->ptr) break; //reached the end.
      else if(rightChild >= h->ptr) min_ind = leftChild; 
      else if(h->a[leftChild] <= h->a[rightChild]) min_ind = leftChild; 	
      else min_ind = rightChild;

      if(h->a[par_ind] > h->a[min_ind]){
	int temp = h->a[min_ind];
	h->a[min_ind]=h->a[par_ind];
	h->a[par_ind] = temp;
	par_ind = min_ind;
      }
      else break; //Heap order is correct now.
    } //actual sift_down

    par_ind--;
  } //sift_down
} //heapify

void hsort(heap *h){//take a heap as input, descending sort the array.
  int c = h->ptr;
  for(int i=0; i<c; i++){
    del(h);
  }
  h->ptr = c; //to do this since ptr decreases on deleting minima from heap.
}//hsort

int main(void){
	heap _min_heap;
	heap *min_heap = &_min_heap; 
        char DEB = 0; //For Debugging purposes
	printf("Enter the number of elements in your array to be Sorted (<1000): ");
	int n;
	scanf("%d", &n);
       	min_heap->ptr=0;
	printf("Enter the elements: \n");
	for(int i=0; i<n; i++){
	  scanf("%d", &(min_heap->a[min_heap->ptr++])); //insert the elements to the min heap without any heap property. Will call heapify later on.
	  if(DEB) printf(">>> Scanned: %d\n", min_heap->a[min_heap->ptr - 1]);
	}//for loop to scan.
	heapify(min_heap);
	hsort(min_heap);
	if(DEB) print_h(min_heap);
	printf("Sorted Array: ");
	for(int i=n-1; i>=0; i--){ //Descending order, hence reverse print the array.
	  printf("%d ", min_heap->a[i]);
          
	}
	printf("\n");
	return 0;
} //main
