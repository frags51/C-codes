/**
 * Data Structures Assignment
 * Binomial Heap Implementation
 * CS16BTECH11038
 */

#include <stdio.h>
#include <stdlib.h>

// ******************************** Binomial Heap Functions **********************************
typedef struct __tnode{ // A single node!
    int val;
    int deg;
    struct __tnode *lChild;
    struct __tnode *rSib;
    struct __tnode *par;
} tnode;

typedef struct{ // Can be represented by just a pointer to the smallest tree's root!
    struct __tnode * HEAD;
} bHeap;

void cleanUpTree(tnode *r){ // free tree rooted at r
    if(r==NULL) return;
    cleanUpTree(r->lChild);
    cleanUpTree(r->rSib);
    free(r);
} //cleanUpTree

/* Make p the parent of c (Makes sense when they have same degree) */
tnode * bTreeLink(tnode *c, tnode *p){ // Link two binomial trees of same degree
    // c, p are roots of thsoe two trees!
    if(c->deg!=p->deg) return NULL;
    c->par = p;
    c->rSib = p->lChild;
    p->lChild = c;
    p->deg++;
    return p;
} // bTreeLink

int add(bHeap *list, tnode *d){ // To add to the bHeap...
    if(list->HEAD == NULL){ //1st element
        list->HEAD = d;
        list->HEAD->rSib=NULL;
        return 1;
    } //else if
    else{ //other elems
        tnode *q=NULL;
        for(q = list->HEAD; q->rSib!=NULL; q=q->rSib);
        q->rSib = d;
        q->rSib->rSib=NULL;
        return 1; //return 1 if inserted
    }//else
}//insert

bHeap * bHeapMerge(bHeap *bh1, bHeap *bh2){ //To merge two heaps (lists) together in increasing order of degree
    bHeap * newHeap = malloc(sizeof(bHeap));
    if(newHeap==NULL) { //Error handling
        fprintf(stderr, "Error in malloc in union!\n");
        return NULL;
    }
    //printf(">>> Alloc'd: %p\n", newHeap);
    newHeap->HEAD=NULL;
    tnode *n1=bh1->HEAD, *n2=bh2->HEAD;
    while(n1!=NULL && n2!=NULL){ // merge while both have elememts
        if(n1->deg <= n2->deg){ //Lower degree goes first.
            tnode *bl = n1->rSib;
            add(newHeap, n1); //this modifies rSib
            n1 = bl;
        }
        else{
            tnode *bl = n2->rSib;
            add(newHeap, n2);
            n2=bl;
        }
    } //while
    while(n1!=NULL){ //check for remaining elements
        tnode *bl = n1->rSib;
        add(newHeap, n1);
        n1=bl;
    }
    while(n2!=NULL){ // check for remaining elements
        tnode *bl = n2->rSib;
        add(newHeap, n2);
        n2=bl;
    }

    return newHeap;
} // bHeapMerge

bHeap * bHeapUnion(bHeap * bh1, bHeap * bh2){ // Union of two heaps!
    bHeap * h = bHeapMerge(bh1, bh2); // Returns a linked list of roots in ascending order
    if(h->HEAD==NULL) return h;
    //printf("h1 was: %p\n", bh1);
    //printf(">>> freed'd: %p\n", bh1);
    //printf(">>> freed'd: %p\n", bh2);
    free(bh1); //Need to free these. Useless now
    free(bh2);
    tnode *prev = NULL;
    tnode *cur = h->HEAD;
    tnode *next = cur->rSib;

    while(next!=NULL){
        if(cur->deg!=next->deg || (next->rSib!=NULL && next->rSib->deg == cur->deg)){ // Case 1, 2 :: CLRS ch 19
            prev = cur;
            cur = next;
        }
        else{ // case 3, 4 :: CLRS
            if(cur->val <= next->val){
                cur->rSib = next->rSib;
                bTreeLink(next, cur);
            }
            else{
                if(prev==NULL) h->HEAD = next;
                else prev->rSib = next;
                bTreeLink(cur, next);
                cur = next;
            } //else cur val > next val
        } // outer else
        next = cur->rSib;
    }
    return h;
} // bHeapUnion

bHeap * bHeapInsert(bHeap *h, tnode *d){ // Insert a tnode into the bHeap
    bHeap * h_temp = malloc(sizeof(bHeap));
    //printf(">>> Alloc'd: %p\n", h_temp);
    d->par=NULL; //Initialize
    d->rSib=NULL;
    d->deg=0;
    d->lChild=NULL;
    h_temp->HEAD=d;
    h=bHeapUnion(h, h_temp);
    return h;
} // bHeapInsert

/* Printing functions */
void tNodePrint(tnode *r){
    if(r==NULL) return;
    printf("%d ", r->val);
    tNodePrint(r->lChild);
    tNodePrint(r->rSib);
} // tNodePrint
void bHeapPrint(bHeap * h){
    tnode *r = h->HEAD;
    if(r==NULL) printf("(Empty)");
    else tNodePrint(r);
    printf("\n");
} //bHeapPrint

int findMin(bHeap * h){ // Just return the min value. O(log n)
    tnode *q=h->HEAD;
    if(q==NULL) return -1;
    int min=q->val;
    for(;q!=NULL; q=q->rSib) if(q->val < min) min = q->val;
    return min;
} //findMin

tnode * extractMin(bHeap ** hPtr){ // return a pointer to min node and remove it form heap.
    const bHeap *g = *hPtr; // Need to modify the pointer heap, which is *pPtr
    tnode *mNode = g->HEAD; // g is const so that I don't accidently modify pointer
    if(g==NULL) return NULL;
    int minVal= g->HEAD->val;
    for(tnode *ctr=g->HEAD;ctr!=NULL;ctr=ctr->rSib){ // find min node
        if(ctr->val < minVal){
            minVal = ctr->val;
            mNode = ctr;
        }
    }
    /* Now remove this node from the root list */
    if(mNode==g->HEAD){
        (*hPtr)->HEAD=(*hPtr)->HEAD->rSib;
    }
    else{ // in this case we are ensured that g->HEAD->next != NULL since mNode exists in the list
        tnode *q = g->HEAD;
        for(tnode *p = g->HEAD->rSib; p!=NULL; q = p, p=p->rSib){
            if(mNode==p){
                q->rSib = p->rSib;
            } //if found
        } //for
    }
    /* Now we need to add this node's children to the heap! */
    bHeap *temp = malloc(sizeof(bHeap));
    //printf(">>> Alloc'd: %p\n", temp);
    temp->HEAD=NULL;
    tnode *t1;
    t1=mNode->lChild;
    if(t1==NULL){ // in case only no child. This node can only be HEAD (deg<this not possible)!
        (*hPtr)->HEAD=mNode->rSib;
        //printf(">>> free'd: %p\n", temp);
        free(temp);
        return mNode;
    }
    tnode *t2=t1->rSib;
    for(; t1->rSib!=NULL && t2!=NULL;){ // Reversing order of linked list of children (To make bheap)
        t1->par=NULL;
        tnode *t3 = t2->rSib;
        t2->rSib = t1;
        t1=t2;
        t2=t3;
    }
    mNode->lChild->par=NULL;
    mNode->lChild->rSib=NULL; // this was not done
    t1->par=NULL; // t1 now points to min degree child of mNode
    temp->HEAD=t1;
    *hPtr = bHeapUnion(*hPtr, temp);
    return mNode;
} // extractMin
// ********************************* MAIN *****************************************************

int main(void){
    char fl=1; //to print help messages.
    /* Declare binomial heap. This is a linked list of binomial trees */
    bHeap *heap=malloc(sizeof(bHeap));
    if(heap==NULL){
        fprintf(stderr, "Error in malloc!");
        return 1;
    }
    heap->HEAD=NULL;

    if(fl) {
        printf("Data Structures Assignment\nCS16BTECH11038\n");
    } //menu print
    while(1){ // menu while
        if(fl){
            printf("\nMENU\n====\n");
            printf("1. Insert\n2. Extract-Min(Find and delete)\n3. Find-min\n4. Print-heap\nOthers: Exit\n");
            printf("Enter your choice: ");
        }
        int ch;
        scanf("%d", &ch);
        switch(ch){
            case 1:{
                if(fl) printf("Enter the value you want to insert: ");
                int v;
                scanf("%d", &v);
                tnode *k = malloc(sizeof(tnode));
                k->val=v;
                heap=bHeapInsert(heap, k);
            } // insert
                break;
            case 2:{
                if(heap->HEAD==NULL) printf("(Empty) Heap\n");
                else{
                    tnode *g=extractMin(&heap);
                    printf("Removed min: %d", g->val);
                    free(g);
                }
            } // extractMin
                break;
            case 3:{ //find-min
                if(heap->HEAD==NULL) printf("(Empty) Heap\n");
                else{
                    printf("Minimum is: %d", findMin(heap));
                }
            } // find-min
                break;
            case 4:{
                printf("The heap is: ");
                bHeapPrint(heap);
            }
            break;
            default:{
                //printf(">>> exiting, this time heap: %p", heap);
                tnode *r=heap->HEAD;
                if(r==NULL) {free(heap);return 0;}
                cleanUpTree(heap->HEAD);
                //printf(">>> free'd: %p\n", heap);
                free(heap);
                return 0;
            } //default
        } //switch ends
    } //menu while
} //main ends
