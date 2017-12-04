/*
Linked LIST GENERALIZED NODE. : Declare the node data on heap.
Run cleanup(llist *) to free everything.

CS16BTECH11038 -- Assignment 1
*/
#include <stdlib.h>

typedef struct __node{ //linked list node!
  void *data; //Generalized thing, can contain data of any type. DYNAMIC ALLOCATION!
    struct __node *next;
} node; //define struct

typedef struct __llist{ //LINKEDLIST!
  node *HEAD;
} llist;
  

int add(llist *list, void *d){
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
        p->data = (void *) d;
        p->next = NULL;
        return 1; //return 1 if inserted
    }//else     
}//insert


int find(llist *list, void *d, int (*comp)(void *, void *)){ //comaprator function that compares 2 values of data.
    if(list->HEAD == NULL) return 0;
    else{
        for(node *p=list->HEAD; p!=NULL; p=p->next){
            if(comp(p->data, d)==0) return 1;                           
        } //for
        return 0;
    } //else
}//find


int del(llist *list, void *d, int (*comp)(void *, void *)){ //deletion
    if(list->HEAD == NULL) return 0;
    else if(comp(list->HEAD->data, d)==0){ //first element is the required one.
      node *temp = list->HEAD->next;
	free(list->HEAD->data);
        free(list->HEAD);
        list->HEAD = temp;
        return 1;
    } // else if
    else{
        node *q = list->HEAD;
        for(node *p = list->HEAD->next; p!=NULL; q = p, p=p->next){
            if(comp(p->data, d)==0){
                q->next = p->next;
		free(p->data);
                free(p);
                return 1;
            } //if found
        } //for
        return 0;
    }//else
            
}//del

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

