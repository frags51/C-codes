/*
Linked LIST
CS16BTECH11038 -- Assignment 1
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define true 1
#define false 0
#define MAX_LEN 50

typedef struct _node{
    char name[MAX_LEN];
    char clg[MAX_LEN];
    char gender;
    struct _node *next;
} node; //define struct

node *HEAD = NULL; //global variable

int insert(char *nm, char *cg, char g){
    node *p = (node *) malloc(sizeof(node));
    if(p==NULL) return false; //check if alloc'd properly
    else if(HEAD == NULL){ //1st element
        HEAD = p;
        strcpy(p->name, nm);
        strcpy(p->clg, cg);
        p->gender = g;
        p->next = NULL;
        return 1;
    } //else if
    else{ //other elems
        node *q=NULL;
        for(q = HEAD; q->next!=NULL; q=q->next);
        q->next = p;
        strcpy(p->name, nm); //strcpy must be used.. char arrays.
        strcpy(p->clg, cg);
        p->gender = g;
        p->next = NULL;
        return 1; //return 1 if inserted
    }//else     
}//insert

int find(char *nm){
    if(HEAD == NULL) return 0;
    else{
        for(node *p=HEAD; p!=NULL; p=p->next){
            if(strcmp(p->name, nm)==0) return 1;                           
        } //for
        return 0;
    } //else
}//find

int del(char *nm){ //deletion
    if(HEAD == NULL) return 0;
    else if(strcmp(HEAD->name, nm)==0){ //first element is the required one.
        node *temp = HEAD->next;
        free(HEAD);
        HEAD = temp;
        return 1;
    } // else if
    else{
        node *q = HEAD;
        for(node *p = HEAD->next; p!=NULL; q = p, p=p->next){
            if(strcmp(p->name, nm)==0){
                q->next = p->next;
                free(p);
                return 1;
            } //if found
        } //for
        return 0;
    }//else
            
}//del

void cleanup(void){ //function to free everything.
    if(HEAD != NULL){
        for(node *p = HEAD; p!=NULL;){
            node *q = p->next;
            free(p);
            p = q;
        } //for loop
    } //if condition.
} //cleanup

int main(void){
    atexit(cleanup); //Free everything at exit.
    printf("Welcome!!!\n");
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
                printf("Enter Name: ");
                char _n[MAX_LEN];
                scanf("%s", _n);
                printf("Enter Clg: ");
                char _clg[MAX_LEN];
                scanf("%s", _clg);
                printf("Enter Gender(M/f): ");
                char _g;
                scanf(" %c", &_g); //Space needed else scanf scans newline char (from previous string entry!)
                if(insert(_n, _clg, _g)) printf("Inserted Successfully!\n");
                else printf("Error While inserting...\n");
            } //insertion block ends
            break;
            case 2:{
                printf("Enter name to find: ");
                char _n[MAX_LEN];
                scanf("%s", _n);
                if(find(_n)) printf("Student named %s found!\n", _n);
                else printf("Student named %s not found...\n", _n);
            }
            break;
            case 3:{
                printf("Enter name to delete: ");
                char _n[MAX_LEN];
                scanf("%s", _n);
                if(del(_n)) printf("Student named %s deleted!\n", _n);
                else printf("Student named %s not found\n", _n);
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