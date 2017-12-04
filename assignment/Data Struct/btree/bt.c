/**
 * Data Structures Assignment
 * CS16BTECH11038
 * B - Tree
*/

#include <stdio.h>
#include <stdlib.h>
#define N 6 //Max number of children ptrs 
// N/2 is min num of children of non leaf non root node
#define minChild (N/2)
//int minChild = N/2; // --> t (Minimum degree)
// Minkeys = minChild - 1: Strictly more means numKeys (i.e node->n) >=minChild

// ************************* B TREE FUNCTIONS **********************************
typedef struct _node{ //These arrays are 1 - indexed for simplicity
    char isLeaf; // If it is a leaf or not
    int n; //No of keys
    int key[N]; //Array of keys
    struct _node *ptr[N+1]; //Array of ptrs to CHILDREN
} node;

node *root;

typedef struct _pair{
    node *a;
    int b;
} pair;

int delete(node *q, const int val); //declaration

pair *find(node *q, int val){
    int i;
    for(i =1; i<=q->n && val > q->key[i]; i++); //get to the next >= index than val
    if(i <= q->n && q->key[i]==val) { //if found, return ptr to node and the index.
        
        pair *t = (pair *) malloc(sizeof(pair));
        t->a = q;
        t->b = i;
        return t;
    }  
    else if(q->isLeaf) return NULL;
    else return find(q->ptr[i], val);
} //find ends

void initTree(void){ //Initialize an empty tree
    node *t = (node *) malloc(sizeof(node));
    t->isLeaf = 1;
    t->n = 0;
    root = t;
} //initTree ends

void splitIChild(node *q, int i){ //Split ith ptr child
    // NOTE : DO WE NEED TO INITIALIZE ARRAY HERE? Ans: NO!
    node *t = (node *) malloc(sizeof(node));
    node *c = q->ptr[i];
    t->isLeaf = c->isLeaf;
     //Min nuumber of children
    t->n = minChild - 1; //Number of keys
    for(int b = 1; b <=  t->n; b++) t->key[b] = c->key[b + minChild]; //Give minChild largest children of q to t     
    if(!c->isLeaf){
        for(int b=1; b<=minChild; b++) t->ptr[b] = c->ptr[b + minChild];
    } //if c is not leaf

    //NOW IN CASE N is EVEN!
    c->n = minChild - 1;
    for(int j = q->n +1; j>=i+1; j--) q->ptr[j+1] = q->ptr[j]; //Copy jth child to j+1th child
    q->ptr[i+1] = t;
    for(int j=q->n; j>=i; j--) q->key[j+1] = q->key[j]; //cpy jth key to j+1th key
    q->key[i] = c->key[minChild];
    q->n++; 
} //splitIChild

void ins_nFull(node *x, int val){ //Insert if root is not full!
    int i = x->n; //Number of keys
    if(x->isLeaf){ //Leaf insertion, all leafs must stay at same level.
        while(i>=1 && val < x->key[i]){ //shift the keys forward till you get to val's position
            x->key[i+1] = x->key[i];
            i--;
        } //while
        x->key[i+1] = val; //insertion
        x->n += 1;
    } //x is Leaf
    else{ //non-leaf node
        for(;i>=1 && val < x->key[i]; i--); //get to the correct position for val, if possible.
        i++; //Since we get to one pos less
        if(x->ptr[i]->n == N-1){ //in case child is full
            splitIChild(x, i);
            if(val > x->key[i]) i++;
        } //if
        ins_nFull(x->ptr[i], val);
    } //else
} //ins nFUll

void insert(int val){ //check if root is full or not
    node *r = root;
    if(r->n == N-1){  
        node *new = (node *) malloc(sizeof(node));
        if(new == NULL) return;
        root = new;
        new->isLeaf=0;
        new->n=0;
        new->ptr[1] = r;
        splitIChild(new, 1);
        ins_nFull(new, val); //new is now the root
    } //if root is full
    else {ins_nFull(r, val);}
}  //insert

void cleanup(node *q){
    if(q->isLeaf) free(q);
    else{
        for(int i=1; i<=q->n+1; i++) cleanup(q->ptr[i]);
        free(q);
    }
}//cleanup

int g_pred(node *q, const int index){ // return predecessor of the q->key[index]
    node *temp = q->ptr[index];  //the left child
    while(!temp->isLeaf) temp=temp->ptr[temp->n+1];

    return temp->key[temp->n]; //the last key (remember indexing from 1!)
} //g_pred

int g_succ(node *q, const int index){ //similarly successor
    node *temp = q->ptr[index+1];
    while(!temp->isLeaf) temp=temp->ptr[1];

    return temp->key[1]; //the first key 
} //g_succ

void delLeaf(node *q, const int i){ //deletion from leaf node. Will assure that delLeaf as atleast minChild-1+1 keys
    for(int j=i+1; j<=q->n; j++) q->key[j-1] = q->key[j];
    q->n--;
} //delLeaf

// MODIFY ROOT case 2c: q is root and non leaf
void delNLeaf(node *q, const int index){
    int val = q->key[index];

    if(q->ptr[index]->n >= minChild){ //In case left child has strictly more than enough keys
        q->key[index] = g_pred(q, index);
        delete(q->ptr[index], q->key[index]);
    } //if
    else if(q->ptr[index+1]->n >= minChild){ //In case right child has strictly more than enough keys
        q->key[index] = g_succ(q, index);
        delete(q->ptr[index+1], q->key[index]);
    }
    else{ //merge right child, index into left child.
        //In case both have == minChild-1 keys.
        node *l = q->ptr[index];
        node *r = q->ptr[index+1];
        //Fill up l
        l->key[minChild] = val; 
        for(int j=1; j<=r->n; j++) l->key[minChild+j] = r->key[j];
        //Also need to copy ptrs.. Note ptrs are not defined if node is leaf.
        if(!l->isLeaf){ //For leaf nodes ptr makes no sense
            for(int j=1; j<=r->n+1; j++) l->ptr[j+minChild] = r->ptr[j];
        }  //if not l is leaf
        //update current node (q)
        for(int j=index+1; j<=q->n; j++) q->key[j-1] = q->key[j];
        for(int j = index+2; j<=q->n+1; j++) q->ptr[j-1] = q->ptr[j]; //index+2 since index+1 == r and is free'd
        q->n--;
        l->n += r->n;
        l->n++;
        if(q==root && q->n==0){ //no key, only one child 
            node *_t = root;
            root = root->ptr[1];//root has only one child now.
            free(_t);
            delete(root, val); //In case root has to removed.
        }
        else delete(q->ptr[index], val); //The value goes to the left child after merging operation 
        free(r); // r is no longer in use!
    }//else
} //delNLeaf

int ensureKeys(node *q, const int index){ //ensure child[i] has enough keys
    char fl=0; //flag to check if root was modified.
    if(index!=1 && q->ptr[index-1]->n >= minChild){ //if left sibling has strictly more than min Keys
        node *from = q->ptr[index-1]; //move from here
        node *to = q->ptr[index]; //move here
        //insert at beginning of to, q->key[index-1].
        for(int j=to->n; j>=1; j--) to->key[j+1] = to->key[j];
        if(!to->isLeaf) for(int j=to->n+1; j>=1; j--) to->ptr[j+1] = to->ptr[j];
        to->key[1] = q->key[index-1];
        if(!to->isLeaf) to->ptr[1] = from->ptr[from->n+1]; //Move last ptr from ==> to
        q->key[index-1] = from->key[from->n]; 
        from->n--;
        to->n++; 
    } //if
    else if(index!=q->n+1 && q->ptr[index+1]->n >=minChild){ //if right sibling has strictly more than min keys
        node *from = q->ptr[index+1]; //move from here
        node *to = q->ptr[index]; //move here
        //insert at end of to, q->key[index].
        to->key[to->n+1] = q->key[index];
        if(!to->isLeaf) to->ptr[to->n+2] = from->ptr[1]; //last ptr <== First ptr
        q->key[index] = from->key[1];
        to->n++;
        for(int j = 2; j<=from->n; j++) from->key[j-1] = from->key[j];
        if(!from->isLeaf) for(int j=2; j<=from->n+1; j++) from->ptr[j-1] = from->ptr[j];
        from->n--; 
    } //else if
    else{ //CASE '3B' : Take care of root
        if(index!=q->n+1){ //in case it is not last child, merge it with next child
            node *l = q->ptr[index]; //this is the node that is the child
            node *r = q->ptr[index+1];
            int val = q->key[index];
            l->key[minChild] = val; //Both have == minChild-1 Keys
            for(int j=1; j<= r->n; j++) l->key[minChild+j] = r->key[j];
            if(!l->isLeaf) for(int j=1; j <= r->n+1; j++) l->ptr[minChild+j] = r->ptr[j];
            l->n += r->n;
            l->n++;
            for(int j=index+1; j<=q->n; j++) q->key[j-1] = q->key[j];
            for(int j = index+2; j<=q->n+1; j++) q->ptr[j-1] = q->ptr[j];
            q->n--;
            if(q==root && q->n==0 && !q->isLeaf){ //modify root!
                node *_t = root;
                root = root->ptr[1];//root has only one child now.
                free(_t); 
                fl=1;
            } //handling root
            free(r);
        } //not last node
        else{ // last child : It is merged with previous child. NOW IT DOESNT EXIST!
            node *l = q->ptr[index-1]; //this is the node that is the child
            node *r = q->ptr[index];
            int val = q->key[index-1]; //Since index==q->n+1 (only q->n keys!)
            l->key[minChild] = val; //Both have == minChild-1 Keys
            for(int j=1; j<= r->n; j++) l->key[minChild+j] = r->key[j];
            if(!l->isLeaf) for(int j=1; j <= r->n+1; j++) l->ptr[minChild+j] = r->ptr[j];
            l->n += r->n;
            l->n++;
            for(int j=index+0; j<=q->n; j++) q->key[j-1] = q->key[j];
            for(int j = index+1; j<=q->n+1; j++) q->ptr[j-1] = q->ptr[j];
            q->n--;
            if(q==root && q->n==0 && !q->isLeaf){ //modify root!
                node *_t = root;
                root = root->ptr[1];//root has only one child now.
                free(_t); 
                fl=1;
            } //handling root 
            free(r);
        } //last node else
    } //else
    return (int)fl;
} //ensureKeys

int delete(node *q, const int val){
    int fIdx = 1;
    pair *_uls = find(q, val); // Just to check if the value is actually there
    if(_uls==NULL) return -1; //not found
    free(_uls);
    for(fIdx=1; fIdx<=q->n && val > q->key[fIdx]; fIdx++); //First index such that key[fIdx] > val
//    printf(">>>Currently fIdx: %d, q->key[fIdx]: %d, val: %d, q: %p\n", fIdx, fIdx<=q->n? q->key[fIdx]:-8, val, q);
    if(fIdx <= q->n && q->key[fIdx]==val){ //Cannot directly use the node, since we need to maintain BT Property
//        printf(">>>found %d in %p\n @ index: %d\n", val, q, fIdx);
        if(q->isLeaf) delLeaf(q, fIdx);
        else delNLeaf(q, fIdx);
        return 0; //Done deletion
    } //if found in this node
    else{ //Not found in this node.
        if(q->isLeaf) return -1; //Not found! (code will not execute this, already checked)
        char ls = fIdx==q->n+1?1:0;//to check for last child and ensure keys
        //ensure keys changes n if we have to delete from last child
        char dd=0; //check if root was modified
        if(q->ptr[fIdx]->n < minChild) {dd=ensureKeys(q, fIdx);}
/*        printf("\n:::DEB:::");
        printf("Root: %p\n", root);
        printf("\nRoot's isLeaf: %d, n: %d value: %d, lChild: %p, rchild: %p",root->isLeaf, root->n, root->key[1], root->ptr[1], root->ptr[2]);
        printf("\n::::DEB ends::::\n");
*/
        if(dd==0){ //in case root not modified
            if(fIdx > q->n+1 && ls) {delete(q->ptr[fIdx-1], val); } //Note that ensureroots changed q->n
            else {delete(q->ptr[fIdx], val);} //Above cases removes last child: merge called (fIdx) && last child (ls)
        }
        else{ //in case root was modified, we know the value is in root
            delete(root, val);
        } //else root was modified
    } //else
    return 0;
} //delete

void print_tree(node *q){
    if(q==NULL) return;
    int i;
    for(i=1; i<=q->n; i++){
        if(!q->isLeaf) print_tree(q->ptr[i]);
        printf("%d ", q->key[i]);
    }
    if(!q->isLeaf) print_tree(q->ptr[i]); //last child: i = n+1
} //print_tree

// ************************************* MAIN ********************************************

int main(void){
    char fl=1;
    if(fl)printf("B-Tree Implementation!\nCS16BTECH11038\n");
    initTree(); //Inititalize the tree
    while(1){ //Menu loop
        if(fl) printf("\nMENU\n====\n1. Insert\n2. Find\n3. Delete\n4. Print tree\nOthers: Exit\nEnter your choice: ");
//        printf("Root starts at: %d, root->n: %d\n", root->key[1], root->n);
        int ch;
        scanf("%d", &ch);
        switch(ch){ //Choice
            case 1:{
                if(fl) printf("Enter the value you want to insert: ");
                int val;
                scanf("%d", &val);
                insert(val);
            } //insert
            break;
            case 2:{
                if(fl) printf("Enter the value you want to find: ");
                int val;
                scanf("%d", &val);
                pair *res = find(root, val);
                if(res==NULL && fl) printf("Not found!\n");
                else printf("Found at index: %d at node: %p\n", res->b, res->a); 
                free(res);
            } //find
            break;
            case 3:{
                if(fl) printf("Enter the value you want to delete: ");
                int val;
                scanf("%d", &val);
                if(delete(root, val)==0 && fl) printf("Successfully deleted %d\n", val);
                else printf("Error: Value %d not found!\n", val);
                if(fl) printf("The tree is now: ");
                root->n==0 ? printf("(Empty)\n") : print_tree(root);
                printf("\n");
            }//delete
            break;
            case 4:{
                if(fl) printf("The tree is: ");
                if(root->n==0) printf("(Empty)"); else {print_tree(root);   
                //printf("\nRoot's isLeaf: %d, n: %d value: %d, lChild: %p, rchild: %p",root->isLeaf, root->n, root->key[1], root->ptr[1], root->ptr[2]);
                printf("\n");            
                } 
            }
            break;
            default:{
                cleanup(root);
                exit(0);
            } //cleanup here
        } //choice switch
    } //while menu
} //main