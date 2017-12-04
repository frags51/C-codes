#include <stdio.h>
#include <stdlib.h>
//define the struct
typedef struct __node{
    int val;
    struct __node *next;
} node;
node* HEAD = NULL;
//function decls.
void print_l(void){
    if(HEAD == NULL) {
        printf("The linked-list is empty! \n");
        return;
    }
    else{
        printf("\n The list is: {");
        for(node *c = HEAD; c!=NULL; c=c->next){
            if(c->next ==NULL) printf(" %d", c->val);
            else printf(" %d,", c->val);
        }
        printf(" }\n");
        return;
    }
}

void ins_beg(int val){
    if(HEAD == NULL){
        HEAD = (node *) malloc(sizeof(node));
        HEAD->val = val;
        HEAD->next = NULL;
        print_l(); //print the list.
    }
    else{
        node *c = (node *) malloc(sizeof(node));
        c->next = HEAD;
        HEAD = c;
        c->val = val;
        print_l();
    }

}//ins_beg

void ins_end(int val) {
    if(HEAD == NULL){
        ins_beg(val);
        return;
    }
    else{
        node *c;
        for(c = HEAD; c->next != NULL; c=c->next); //Reach last element.
        c->next = (node *) malloc(sizeof(node));
        (c->next)->next = NULL;
        (c->next)->val = val;
        print_l();
    }
}//ins_end

void ins_aft(int val, int key){
    if(HEAD == NULL){
        printf("ERROR! The linked-list is empty! \n");
        return;
    }   
    else{
        node *c;
        for(c = HEAD; c!=NULL; c=c->next) if(c->val == key) break;
        
        if(c==NULL) printf("ERROR! Element with value %d not found!\n", key);
        else{
            node *d = (node *) malloc(sizeof(node));
            d->next = c->next;
            c->next = d;
            d->val = val;
            print_l();
            return;
        }
    } //else
} //ins_aft
void del_all(void){
    if(HEAD == NULL){
        printf("ERROR: The linked-list is empty! \n");
        return;
    }
    //no need of else.
    for(node *c = HEAD; c!=NULL; ){
        node *d = c->next;
        free(c);
        c = d;
    }

    HEAD = NULL; //Make HEAD NULL AGAIN.
    //printf("The linked list is empty now!");
    print_l();
} //del_all

void del_all_exit(void){ //executed at exit!
    if(HEAD == NULL){        
        return;
    }
    //no need of else.
    for(node *c = HEAD; c!=NULL; ){
        node *d = c->next;
        free(c);
        c = d;
    }

    HEAD = NULL;
} //del_all_exit

void del_at(int key){
    if(HEAD == NULL) printf("ERROR! The linked-list is empty! \n");
    else{
        node *c, *prev=NULL, *last_occ = NULL; //prev==HEAD since to cater only 1 element array(or key is at pos 1).
        for(c=HEAD; c!=NULL; c=c->next){ //note : can't reach last element using this.
            if(c->val == key) {last_occ = prev; }
            prev = c;
            
        }
        if(last_occ==NULL && HEAD->val != key) printf("ERROR! Element not found!\n"); //
        else if(last_occ==NULL && HEAD->val == key){
            prev = HEAD;    
            HEAD = HEAD->next; //by loop condition c!=NULL.
            free(prev);     
            print_l();
        }
        else{
            prev = last_occ->next; //prev is a dummy variable now.
            last_occ->next = (last_occ->next)->next; //last_occ->next exists per the code!
            free(prev);
            print_l();
        }    
    } //HEAD == NULL check
}//del_at

//

int main(void){
    system("clear");
    atexit(del_all_exit);
    printf("CS16BTECH11038 -- CS1353 -- Assignment 2\n\n");
    printf("LINKED LISTS\n");

    //menu
    while(1){
        printf("\n");
        printf("1. Insert node at end of list.\n");
        printf("2. Insert node at start of list.\n");
        printf("3. Insert node After first occurence of specified value,\n");
        printf("4. Delete a node that contains the last occurence of specified value.\n");
        printf("5. Delete all elements of the linked-list.\n");
        printf("6. Print all the values of the linked-list.\n");
        printf("7. Exit\n");
        printf("Enter your response: ");

        int choice=0;
        scanf("%d", &choice);

        switch(choice){
            case 1:{
            int val = 0;
            printf("Enter value to be inserted: ");
            scanf("%d", &val);    
            ins_end(val);
            }
            break;

            case 2:{
            int val = 0;
            printf("Enter value to be inserted: ");
            scanf("%d", &val);    
            ins_beg(val);
            }
            break;

            case 3: {
            int aft = 0, val=0;
            printf("Enter the value(to be added) <space> The key to be searched for: ");
            scanf("%d %d",&val, &aft);
            ins_aft(val, aft);
            }
            break;

            case 4: {
            int aft = 0;
            printf("Enter the value to be searched: ");
            scanf("%d",&aft);
            del_at(aft);
            }
            break;

            case 5:
            del_all();
            break;

            case 6:
            print_l();
            break;

            case 7:
            exit(0); //atexit should take care of all the freeing.
            break;

            default:
            printf("Please enter a valid value.\n");
            break;
        }
    }
    // menu ends    
} //main