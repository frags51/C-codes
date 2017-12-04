/**
 * Data Structures Assignment
 * CS16BTECH11038
 * Red-Black Tree (Insert Operation)

 */ 

#include <stdio.h>
#include <stdlib.h>
//Struct for storing node.
//Color red = 1, black = 0
typedef struct __node{
    int val;
    char col; //only 2 values needed for color - 1 Byte only.
    struct __node * l; // Left
    struct __node * r; // Right
    struct __node * p; //Parent   
} node;

node *root; //The root node for the tree.
node *sent; //Sentinel node : Colored Black.

//********************* TREE FUNCTIONS ****************************
void left_rot(node *x){ 
    node *q = x->r; //Left rot assumes x->r != Sentinel node
    x->r = q->l;
    if(q->l != sent) q->l->p = x;
    q->p = x->p; //Set q's parent as that of x's parent
    if(x->p == sent) root = q;
    else if(x == x->p->l) x->p->l = q; //in case x is left subchild
    else x->p->r =  q;
    q->l = x;
    x->p = q;
} //left_rot endss

void right_rot(node * x){
    node *q = x->l;
    x->l = q->r;
    if(q->r != sent) q->r->p = x;
    q->p = x->p;
    if(x->p == sent) root = q;
    else if(x == x->p->l) x->p->l = q;
    else x->p->r=q;
    q->r = x;
    x->p = q;
} //right_rot ends

void balance(node *a){ //1 -> RED, 0-> BLACK
    node *e; //For storing uncle
    while(a->p->col == 1){
        if(a->p == a->p->p->l){//In case a's parent is left subchild.
            e = a->p->p->r;
            if(e->col == 1){ //Red uncle
                a->p->col = 0;
                e->col = 0;
                a->p->p->col = 1;
                a = a->p->p; // set a = grandparent
            } //Red uncle
            else {
                if(a == a->p->r){ //black uncle, and it itself is right subchild
                    a = a->p;
                    left_rot(a);
                } //black uncle and itself right subchild
                a->p->col = 0;
                a->p->p->col = 1;
                right_rot(a->p->p); 
            } //else ends
        } // A's parent is left subchild
        else{ //a's parent is right subchild 
            e = a->p->p->l; //e is uncle
            if(e->col == 1){ //Red uncle
                a->p->col=0;
                e->col=0;
                a->p->p->col = 1;
                a = a->p->p;
            } //Red Uncle
            else{
                if(a == a->p->l){ //Black uncle, itsef left subchild
                   a = a->p;
                   right_rot(a); 
                }
                a->p->col = 0;
                a->p->p->col = 1;
                left_rot(a->p->p);
            } //else ends
        } //a's paren is right subchild
    } //while loop ends
    root->col=0;
} //balance ends
 
node* insert(const int value){
    node *new = (node*) malloc(sizeof(node));
    if(new==NULL) return NULL; //in case allocation fails.
    new->val = value;
    node *x = root;
    node *y = sent;
    while(x!=sent){ //traverse to end of tree according to bst properties.
        y = x;
        if(new->val < x->val ) x = x->l;
        else x = x->r;        
    } //while
    new->p = y;
    if(y==sent) root = new; //in case root is NULL
    else if(new->val < y->val) y->l = new;
    else y->r = new;
    new->l = sent;
    new->r = sent;
    new->col=1; //Set color red
    balance(new);
    return new;
} //insert ends

void cleanup(node *a){
    if(a==sent) return;
    cleanup(a->l);
    cleanup(a->r);
    //printf("Freeing.. %d\n", a->val);
    free(a);
}

void inorder(node *a){
    //printf("In Inorder!\n");
    //printf("a: %p", a);
    if(a!=sent){
        inorder(a->l);
        printf("%d:%c ", a->val, (a->col == 0 ? 'B': 'R'));
        inorder(a->r);
    }
} //inorder
// *********************** MAIN **********************

int main(void){
    //atexit(cleanup);
    char fl=1; //To print help messages.
    sent = (node *) malloc(sizeof(node));
    sent->l=sent;
    sent->r=sent;
    sent->p=sent;
    sent->col = 0; //black colored.
    sent->val=0;

    root = sent; //Initialization
    
    while(1){
        if(fl){
            printf("\n****R-B Tree Insert operations!****\n\n");
            printf("1. Insert value\n2. Inorder traversal (Along with color): ");
        }
        int ch;
        scanf("%d", &ch);
        switch(ch){
            case 1:{
                if(fl) printf("Enter value to insert: ");
                int k;
                scanf("%d", &k);
                if(insert(k) == NULL && fl==1) printf("Insertion failed! \n");
            }  
            break;
            case 2:{
                if(fl) printf("\nInorder: ");
                inorder(root);
                printf("\n");
            }
            break;
            default:{
                cleanup(root);
                free(sent);
                exit(0);
            }
        } //switch
    } //menu while
} //main ends