/**
Supreet Singh
CS16BTECH11038
Disjoint Set Union
Data Structures Assignment
PLEASE NOTE THAT ARRAY INDICES HERE START FROM ZERO (0)
Need to initialize all members of struct dsu
*/

#include <stdio.h>

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

void union_dsu(dsu *a, const int x, const int y){ //Union of sets having elements a and b.
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

int main(void){
    printf("Disjoint Set Union\nEnter Number of elements (NOTE: INDEX STARTS FROM 0 (ZERO): ");
    dsu set; //set is an object of type dsu
    scanf("%d", &set.num);
    int par[set.num];
    int sz[set.num];
    set.p = par;
    set.size = sz;
    init_dsu(&set);
    while(1){ //while loop to act as menu for union/find
        printf("Please make a choice: \n");
        printf("1. Union\n2. Find\nOther: Exit. ");
        int ch;
        scanf("%d", &ch);
        switch(ch){
            case 1:
            {
            printf("Enter two elements to make a union of: ");
            int x, y;
            scanf("%d %d", &x, &y);
            union_dsu(&set, x, y);
            }
            break;
            case 2:
            {
            printf("Enter element to find parent of: ");
            int x;
            scanf("%d", &x);
            x = find_dsu(&set, x);
            if(x==-1) printf("Enter valid element... \n");
            else printf("The parent is: %d\n", x);
            }
            break;
            default: return 0;
        }
    } //while ends
    return 0;
}