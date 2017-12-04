/**
 * Data Structures Assignment
 * CS16BTECH11038
 * Kruskal's Algorithm 
 * Input: Vertices Edges
 * All edges:   V1 V2 Wt  
 * Vertices start from 1!
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct _edge{ //This struct stores the graph
    int u;
    int v;
    int wt;
}edge; //struct for taking weighted edge as an input.


//**************QUICKSORT*************************
void swapAt(edge *arr[], const int i, const int j){
    edge *temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}//swapAt

int partition(edge *arr[], int lo, int hi, int pivot){
        int i = lo-1, j=lo;
        swapAt(arr, pivot, hi); //Shift pivot to last spot.
        int p_elem = arr[hi]->wt; //Now pivot is at hi!
        for(; j<hi; j++) if(arr[j]->wt < p_elem) {i++; swapAt(arr, i, j);}
        swapAt(arr,i+1, hi); //bring pivot to i+1th pos.(
        return i+1;
}

void qsort_l(edge *arr[], int lo, int hi){ //last element is pivot.
    if(lo<hi){
        int j = partition(arr, lo, hi, hi);
        qsort_l(arr, lo, j-1);
        qsort_l(arr, j+1, hi);
    } //check for 1 elemnt.
}

// **************Quicksort ends*********************


//**************DSU*******************************
typedef struct _dsu{
    int *p; //parent of each element
    int *size; //Sizes of each set
    int num; //Store number of elements in the DSU
} dsu;

void init_dsu(dsu *a){ //Initialize the struc
    for(int i=0; i< a->num; i++) {
        a->p[i] = i; //Initially all are their own parents.
        a->size[i] = 1; //Initially all are size 1.
    } 
}  

int find_dsu(dsu *a, int i){ //Find the parent of element 'i'
    if(i >= a->num){
        return -1;
    }
    while(a->p[i]!=i) {
        a->p[i]=a->p[a->p[i]]; //Path compression, set parent of i to be grand-parent of i.
        i = a->p[i];
    }
    return i;
} //find_dsu ends

void union_dsu(dsu *a, int x, int y){ //Union of sets having elements a and b.
    int p_x = find_dsu(a, x);
    int p_y = find_dsu(a, y);
    if(a->size[p_x] < a->size[p_y]){ //Size of x is less than that of y.
        a->p[p_x] = a->p[p_y];
        a->size[p_y] += a->size[p_x]; //Set parent of Y as parent of parent of X.
    }
    else{
        a->p[p_y] = a->p[p_x];
        a->size[p_x] += a->size[p_y]; //Set parent of Y as parent of parent of X.        
    } //All this to balance the tree 
}//union_dsu ends


//**************Main Function*******************************

int main(void){
    char fl = 1; //To enable printing of help messages
    //v :  No of vertices, e: No of edges in input graph.
    if(fl){
        printf("Implementation of Kruskal's Algorithm.\n");
        printf("Enter Number of vertices in the graph: ");
    }
    int v;
    scanf("%d", &v);
    if(fl){
        printf("Enter the number of edges in the graph: ");
    }
    int e;
    scanf("%d", &e);
    edge *arr[e]; //An array to store all these edges.
    if(fl){
        printf("Enter the edges as: V1 V2 Weight: ");
    }
    for(int i=0; i<e; i++) {
        arr[i] = (edge *) malloc(sizeof(edge));
        scanf("%d %d %d", &(arr[i]->u), &(arr[i]->v), &(arr[i]->wt));
        arr[i]->u--;
        arr[i]->v--; //since vertices are given to start from 1
    }

    qsort_l(arr, 0, e-1); //sort the edges by weight.
    /* Initialize DSU struct */
    dsu set;
    set.num = v; //set has v vertices
    int par[v];
    int sz[v];
    set.p = par;
    set.size = sz;
    init_dsu(&set);
    edge *mst[v-1]; //MST has v-1 edges
    int mst_ptr = 0;
    for(int i=0; i<e; i++){ //for loop to iterate over edges
        int a = find_dsu(&set, arr[i]->u);
        int b = find_dsu(&set, arr[i]->v);
        if(a!=b){ //If these are not yet connected
            //Add this edge to the mst.
            /*mst[mst_ptr]->u = arr[i]->u;
            mst[mst_ptr]->v = arr[i]->v;
            mst[mst_ptr]->wt = arr[i]->wt;
            */
            mst[mst_ptr] = arr[i];
            mst_ptr++; 
            union_dsu(&set, a, b);
        }
        //else do nothing
    } //for loop ends

    if(fl) printf("The list of edges of the MST is: \n");
    for(int i=0; i < v-1; i++){ //Output the edges in the MST

        printf("%d %d %d\n", mst[i]->u + 1, mst[i]->v + 1, mst[i]->wt); //To make vertices index from 1 again
    }

    for(int i=0; i<e; i++) free(arr[i]); //free the dynamically allocated memory
    return 0;
} //main ends
