/*
CS16BTECH11038 -- Assignment 1
Binary Search Tree
Uses an integer as the key
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define true 1
#define false 0
#define MAX_LEN 50 //For max length of strings

typedef struct _node{
    int name;
    char clg[MAX_LEN];
    char gender;
    struct _node *l; //left subtree pointer
    struct _node *r; //right subtree pointer
} node; //define struct

node *root = NULL; //global variable

int insert(int nm, char *cg, char g){
    node *p = (node *) malloc(sizeof(node));
    if(p == NULL) return 0;
    else if(root == NULL){ //first element
        root = p;
        root->name = nm;
        strcpy(root->clg, cg);
        root->gender = g;
        root->l=NULL;
        root->r=NULL;
        
    }//else if
    else{ //other elements.
        node *q = root;
        while(1){
            if(nm < q->name && q->l!=NULL){
                q = q->l;
                continue;
            }
            else if(nm < q->name){
                q->l = p;
                p->name = nm;
                p->gender = g;
                strcpy(p->clg, cg);
                p->l = NULL;
                p->r = NULL;
                break;
            }
            else if(nm > q->name && q->r!=NULL){
                q = q->r;
                continue;
            }
            else if(nm > q->name){
                q->r = p;
                p->name = nm;
                p->gender = g;
                strcpy(p->clg, cg);
                p->l = NULL;
                p->r = NULL;
                break;
            }        
        } //while loop        
    } //else      
    return 1;  
}//insert

node* find(int nm){
    if(root==NULL) return NULL;
    node *q  = root;
    while(1){
        if(q->name == nm) return q;
        else if(nm < q->name){
            if(q->l == NULL) return NULL; //not found
            q = q->l;
        } 
        else{ //nm > q->name
            if(q->r == NULL) return NULL;
            q = q->r;
        }
    }//while
}//find

node* del(node *p, int nm){ //deletion
    if(find(nm)==NULL) return (void *)-1;
    if(p==NULL) return NULL;

    if(nm < p->name){
        p->l = del(p->l, nm);
    }
    else if(nm > p->name){
        p->r = del(p->r, nm);
    }
    else{ // nm = p->name, we found the match
        if(p->l==NULL && p->r==NULL) {if(p==root) root=NULL; free(p); p=NULL;} //leaf node -- NO CHILD
        else if(p->l==NULL){
            node *q = p;
            if(p==root) root = root->r; //IMP!!!
            p = p->r;
            free(q);
        }
        else if(p->r==NULL){
            node *q = p;
            if(p==root) root = root->l; //NOTE:: C function makes copies of arguments. So a copy of root is made. ROOT IS NOT CHANGED!!
            p=p->l;
            free(q);
        }
        else{
            node *q = p->r;
            for(; q->l!=NULL;q=q->l); //find minimum child on right side.
            p->name = q->name;
            strcpy(p->clg, q->clg);
            p->gender = q->gender;
            p->r=del(p->r, q->name);
        }

    } //match found
    return p;        
}//del

void print_tree(node *p){
    if(p!=NULL){
        print_tree(p->l);
        printf("%d ", p->name);
        print_tree(p->r);

    }
} //print tree

void cleanup(node *p){ //function to free everything. //recursion
    if(p==NULL) return;
    cleanup(p->l);
    cleanup(p->r);
    free(p);
} //cleanup

void cleanup2(){ //required.. atexit requires function without arguments
    cleanup(root);
}
int main(void){
    atexit(cleanup2); //Free everything at exit.
    printf("Welcome!!!\nNote: Name is an integer!\n");
    while(true){ // infinite menu
        printf("Enter your choice!\n");
        printf("1. Insert a new student.\n");
        printf("2. Search for a student.\n");
        printf("3. Delete a student.\n");
        printf("4. Print Tree.\n");
        printf("5. Exit\n");
        int ch;
        scanf("%d", &ch);
        switch(ch){
            case 1:
            { //insertion
                printf("Enter Name (Integer): ");
                int _n;
                scanf("%d", &_n);
                //printf("Enter Clg: ");
                char _clg[MAX_LEN];
                //scanf("%s", _clg);
                //printf("Enter Gender(M/f): ");
                char _g;
                //scanf(" %c", &_g); //Space needed else scanf scans newline char (from previous string entry!)
                _clg[0] = 'A'; _clg[1] = '\0'; _g = 'M'; // Just fill these values for now!
                if(insert(_n, _clg, _g)) printf("Inserted Successfully!\n");
                else printf("Error While inserting...\n");
            } //insertion block ends
            break;
            case 2:{
                printf("Enter name to find: ");
                int _n;
                scanf("%d", &_n);
                if(find(_n)) printf("Student named %d found!\n", _n);
                else printf("Student named %d not found...\n", _n);
            }
            break;
            case 3:{
                printf("Enter name to delete: ");
                int _n;
                scanf("%d", &_n);
                if(del(root,_n)!=(void *)-1) printf("Student named %d deleted!\n", _n);
                else printf("Student named %d not found\n", _n);
            }
            break;
            case 4: {
                printf("Tree: ");
                if(root==NULL) printf("(Empty)");
                else print_tree(root);
                printf("\n");
            }
            break;
            case 5: {
                exit(0);
            }
            break;
            default: printf("Enter a valid choice... \n");
        } //switch
    } //while loop
} //main ends