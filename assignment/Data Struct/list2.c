/*
Linked LIST
CS16BTECH11038 -- Assignment 1
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define true 1
#define false 0

typedef struct _node{ //linked list node!
  void *data; //Generalized thing, can contain data of any type. DYNAMIC ALLOCATION!
    struct _node *next;
} node; //define struct

typedef struct __llist{ //LINKEDLIST!
  node *HEAD;
} llist;
  

int add(llist *list, void *d){
    node *p = (node *) malloc(sizeof(node));
    if(p==NULL) return false; //check if alloc'd properly
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

int comp(void *a, void *b){
  if(*(int *)a == * (int *)b) return 0;
  else return 1;
}

int main(void){
  // atexit(cleanup); //Free everything at exit.
    printf("Welcome!!!\n");
    llist __l;
    llist *l = &__l;
    l->HEAD=NULL; //Needs to be initialized to NULL. 
    while(true){ // infinite menu
        printf("Enter your choice!\n");
        printf("1. Insert a new student.\n");
        printf("2. Search for a student.\n");
        printf("3. Delete a student.\n");
        printf("4. Exit.\n");
        int ch;
        scanf("%d", &ch);
        switch(ch){
            case 1:
            { //insertion
	      printf("Enter number: ");
	      int *d = (int *) malloc(sizeof(int));
	      scanf("%d", d);
	      if(add(l, d)) printf("Inserted Successfully!\n");
                else printf("Error While inserting...\n");
            } //insertion block ends
            break;
            case 2:{
                printf("Enter namenum to find: ");
                int v;
                scanf("%d", &v);
                if(find(l, &v, comp)) printf("Student numbered %d found!\n", v);
                else printf("Student numbered %d not found...\n", v);
            }
            break;
            case 3:{
                printf("Enter name to delete: ");
                 int v;
                scanf("%d", &v);
                if(del(l,&v, comp)) printf("Student num %d deleted!\n", v);
                else printf("Student named %d not found\n", v);
            }
            break;
            case 4: {
	      
                exit(0);
            }
            break;
            default: printf("Enter a valid choice... \n");
        } //switch
    } //while loop
} //main ends
