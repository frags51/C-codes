/**
 * Data Structures Assignment
 * Implementation of Prim's Algorithm
 *  CS16BTECH11038
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h> 

#define HEAP_SIZE 1000

typedef struct __vertex{ 
    int name; //Which vertex this is
    int val; //the value to compare by!
    struct __vertex *p; //It's parent!
    char in_heap;
} vertex;

//****************** MIN - HEAP FUNCTIONS ********************
// MIN - HEAP Holds data of type struct __vertex

typedef struct _heap{
	vertex **a; //Pointer to an array of pointer to vertices! (technically pointer to pointer, bleh)
	int ptr; //next position for element to be inserted
} heap;

//functions
int hAdd(heap * h, vertex *v){
	if(h->ptr >= HEAP_SIZE) return 0;
    h->a[h->ptr++] = v;
    v->in_heap = 1;
	int c = h->ptr - 1;
	int p = (c-1)/2; // parent  = (leaf-1/)2
	while(p>=0){//bubble up
		vertex *temp = h->a[c];
		if(h->a[c]->val < h->a[p]->val){
			h->a[c] = h->a[p];
			h->a[p] = temp;
		}
		else break;
		//int t2 = p;
		c = p;
		p = (p-1)/2;	 
	}//while
	return 1; //added successfully
} //add.

vertex *extractMin(heap *h){ //extract-min
	if(h->ptr == 0) return NULL;
    h->ptr--;
    vertex *temp = NULL;
	if(h->ptr > 0){
        int c = h->ptr;
        
        h->a[0]->in_heap = 0; 
        //Swapping code
        temp = h->a[0];
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
				if(h->a[leftChild]->val <= h->a[rightChild]->val) min_index = leftChild;
				else min_index = rightChild;
			}
			if(h->a[now]->val > h->a[min_index]->val){
				vertex *t2 = h->a[min_index];
				h->a[min_index] = h->a[now];
				h->a[now] = t2;
				now = min_index;
			}
			else break;
		} //sift down loop!
    }
    else {temp = h->a[0]; h->a[0]->in_heap=0;}
	return temp;
}// extract-min
int isEmpty(heap *h){ //only to test if it's not empty.
    return h->ptr > 0;
}//peek
void print_h(heap *h){
	for(int i=0; i<h->ptr; i++) printf("%d ", h->a[i]->val);
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
        else if(h->a[leftChild]->val <= h->a[rightChild]->val) min_ind = leftChild; 	
        else min_ind = rightChild;
  
        if(h->a[par_ind]->val > h->a[min_ind]->val){
      vertex *temp = h->a[min_ind];
      h->a[min_ind]=h->a[par_ind];
      h->a[par_ind] = temp;
      par_ind = min_ind;
        }
        else break; //Heap order is correct now.
      } //actual sift_down
  
      par_ind--;
    } //sift_down
  } //heapify

// ******************* LINKED LIST ***********************************

typedef struct __pair{ //for edge
    int v;
    int wt;
} pair;

typedef struct __node{ //linked list node!
    pair *data; //Generalized thing, can contain data of any type. DYNAMIC ALLOCATION!
      struct __node *next;
  } node; //define struct
  
  typedef struct __llist{ //LINKEDLIST!
    node *HEAD;
  } llist;
    
  
  int add(llist *list, pair *d){
    
    node *p = (node *) malloc(sizeof(node));
      
    if(p==NULL) return 0; //check if alloc'd properly
      else if(list->HEAD == NULL){ //1st element
          list->HEAD = p;
          p->data = (void *) d;
      p->next = NULL;
      
          return 1;
      } //else if
      else{ //other elems
          node *q=NULL;
          for(q = list->HEAD; q->next!=NULL; q=q->next);
          q->next = p;
          p->data = d;
          p->next = NULL;
          return 1; //return 1 if inserted
      }//else     
  }//insert
  
  
  void cleanup(llist *list){ //function to free everything.
      if(list->HEAD != NULL){
          for(node *p = list->HEAD; p!=NULL;){
              node *q = p->next;
              free(p->data);
          free(p);
              p = q;
          } //for loop
      } //if condition.
  } //cleanup

// ********************** MAIN FUNCTION **********************************

int main(void){
    char fl = 1; //For printing help messages.
    if(fl) printf("Prim's Algorithm\nCS16BTECH11038 - Supreet Singh\n");
    if(fl) printf("Enter number of vertices: ");
    int v; 
    scanf("%d", &v);

    //USING BLAH, we these pointers can be easily modified since we know the address of each vertex by address
    //We create a copy of these addresses for heap functions to play around with (Change ordering etc).
    
    vertex *blah[v]; //An array of vertices to be placced in min-heap
    for(int i=0; i<v; i++){ //to initialize these vertices
        blah[i] = (vertex *) malloc(sizeof(vertex));        
        blah[i]->name = i; //this is the i'th vertex
        blah[i]->val = INT_MAX; //Infinity equivalent
        blah[i]->p = NULL;
        blah[i]->in_heap = 1;
    } //for

    heap *h = (heap *) malloc(sizeof(heap)); //allocate space for heap.
    h->a = (vertex **) malloc(v * sizeof(vertex *));
    h->ptr = v; 
    for(int i=0; i<v; i++) {h->a[i] = blah[i];}
    llist *adj[v]; //adjacency list

    for(int i=0; i<v;i++) {adj[i] = (llist *)malloc(sizeof(llist)); } //Need to set all Heads to null.

    for(int i=0; i<v; i++) {adj[i]->HEAD=NULL; } //Need to set the head of each to NULL
   
    
    if(fl) printf("Enter number of edges: ");
    
    int e; 
    scanf("%d", &e); //get the number of edges
    if(fl) printf("Enter the edges as: V1 V2 Weight: ");
    for(int i=0; i < e; i++){ //For loop to add to adjacency lists
        int a, b, c;
        scanf("%d %d %d", &a, &b, &c);

        a--; //vertices start at 1, but code handles start from 0.
        b--;
        
        pair *p1 = (pair *) malloc(sizeof(pair));
        p1->v = b; p1->wt=c;
        add(adj[a], p1);
//        printf("*** p1 = NULL?: %s", p1==NULL?"NULL!":"Not nULL!");
  //      printf("added to adj of %d: %d with wt %d\n", a, p1->v, p1->wt);
        pair *p2 = (pair *) malloc(sizeof(pair));
        p2->v = a; p2->wt=c;
        add(adj[b], p2);   
        
    }

    //Prim's Algo begins
    int p_root = 0; //The vertex to start prims's algorithm at.
    blah[p_root]->val=0; //set it's value to zero.
    heapify(h);

    //    printf("**HEAPIFY!\n");
    //FOR DEBUGGING: Print the adj lists
    /*printf("ADJ lists \n");
    for(int i=0; i<v; i++){
        node *p = adj[i]->HEAD;
        printf("adj list of %d: ", i);
        for(; p!=NULL; p=p->next){
            printf("%d ", ((pair *)p->data)->v);
        }
        printf("\n");
    }
    */
    
    while(h->ptr > 0){ //While heap is NOT EMPTY
        vertex *a = extractMin(h);
        node *p = adj[a->name]->HEAD;
        for(; p!=NULL; p=p->next){ //To iterate over adjacency list
            //printf("CRRCT: %d\n", ((pair *)p->data)->v + 1);
            if(blah[p->data->v]->in_heap && (p->data)->wt < blah[(p->data)->v]->val){
                blah[(p->data)->v]->val = (p->data)->wt;
                blah[(p->data)->v]->p = a;
                heapify(h); //Needed at each step to maintain heap property.
                //NOTE
                // blah[((pair *)p->data)->v] points to a vertex whose index (type pair.v) is stored in llist->data
            } //if ends
        } //for loop - iterate over adj-list
    } //while loop

    //PRINT THE MST
    for(int i=0; i<v; i++){
        if(i==p_root) continue; // MST only has v-1 edges.
        printf("%d %d %d\n",blah[i]->name + 1, blah[i]->p->name + 1, blah[i]->val); 
        //This code starts indexing from 0, so +1
    } //to print the MST
    
    for(int i=0; i<v; i++) free(blah[i]);
//    for(int i=0; i<v; i++) free(h->a[i]);
    free(h->a);
    free(h);
    for(int i=0; i<v; i++) {cleanup(adj[i]); free(adj[i]);}
    return 0;
} //Main ends