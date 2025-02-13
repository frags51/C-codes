//MIN-HEAP
//CS16BTECH11038
// h->ptr : NEXT AVAILABLE FREE SPACE!
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
	if(h->ptr >= HEAP_SIZE) return 0;
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
		temp = p;
		c = p;
		p = (p-1)/2;	 
	}//while
	return 1; //added successfully
} //add.

int extractMin(heap *h){ //p.s. THIS IS NOT EXTRACT MIN (It is similar)
	if(h->ptr == 0) return 0;
	h->ptr--;
	int temp;
	if(h->ptr > 0){
		temp = h->a[0];
		int c = h->ptr;
		h->a[0] = h->a[c];
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
			if(h->a[now] > h->a[min_index]){
				int temp = h->a[min_index];
				h->a[min_index] = h->a[now];
				h->a[now] = temp;
				now = min_index;
			}
			else break;
		} //sift down loop!
	}
	else temp = h->a[0];
	return temp;
}// del
int peek(heap *h){ //only to test if it's not empty.
	return h->ptr > 0;
}//peek
int findMin(heap *h){
	if(peek(h)) return h->a[0];
	else return 0;
}
void print_h(heap *h){
	for(int i=0; i<h->ptr; i++) printf("%d ", h->a[i]);
	printf("\n");
}

//
int main(void){
	heap _min_heap;
	heap *min_heap = &_min_heap; 
	min_heap->ptr=0;
	while(1){
		printf("\nMIN HEAP :: Enter Your Choice:\n");
		printf("1. Add to heap.\n2.Extract-min (Delete and return min) from heap\n3. Find Min\n4.Print Heap\nAnyOther: Exit\n\nYour choice: ");
		int ch;
		scanf("%d", &ch);
		switch(ch){
			case 1:{
				printf("Enter the element you want to add: ");
				int n;
				scanf("%d", &n);
				
				if(add(min_heap, n)) printf("Added!\n");
			}//case 1
			break;
			case 2:{
				if(peek(min_heap)) printf("The min element is: %d!\n", extractMin(min_heap));
				else printf("Heap is empty...\n");
			}
			break;
			case 3:{
				if(peek(min_heap)) printf("The minimum element is: %d\n", findMin(min_heap));
				else printf("Heap is empty...\n");
			}
			break;
			case 4:{
				if(min_heap->ptr > 0) for(int i=0; i< min_heap->ptr; i++) printf("%d ", min_heap->a[i]);
				else printf("Heap is empty...\n");
			}
			break;
			default:
				exit(0);
		} //switch
	} //menu while loop
} //main