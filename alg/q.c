//Queue.
// % mod not defined for -ve numbers
#include <stdio.h>
#include <stdlib.h>
#define SIZE 5
int arr[SIZE];
int sPtr = 0;
int ePtr= 0; 
//Circular Q:
void nq(int a){
    if((ePtr % SIZE) == ((sPtr - 1 + SIZE) % SIZE)) {
        printf("Queue Full!\n");
        return;
    }
    arr[ePtr] = a;
    ePtr++;
    ePtr %= SIZE;
    printf("ePtr_now: %d   ", ePtr);
    printf("sPtr_now: %d   ", sPtr);
    return;
}
int dq(void){
    if(ePtr == sPtr) {
        printf("Queue Empty! \n");
        return 0;
    }

    sPtr++;
    sPtr %= SIZE;
       printf("ePtr_now: %d   ", ePtr);
    printf("sPtr_now: %d   ", sPtr);
    return arr[(sPtr - 1 + SIZE) % SIZE];
}
void prt(void){
    if(ePtr == sPtr) {
        printf("Queue Empty! 2\n");
        return;
    }
    int i = sPtr;
    while(1){
        if((i % SIZE) == ePtr) break;
        else /*((i % SIZE) < ePtr)*/ printf("%d ", arr[i % SIZE]);
        //else pgrintf("Invalid : i%%SIZE > ePtr");
        i++;
    }
    printf("\n");
}
int main(void){
    int ch = 1;
    while(ch == 1 || ch ==2 || ch == 3 || ch ==4){
        printf("\nNQ: 1 , DQ: 2, Prt: 3\n");
        scanf("%d", &ch);
        

        switch(ch){
            case 1: {
            printf("Enter elem: ");
            int elem;
            scanf("%d", &elem);
            nq(elem);
            prt();
            }
            break;
            case 2: {
                printf("Dq'd: %d\n",dq());
                prt();
            }
            break;
            case 3:{
                prt();
            }
            break;
            case 4: {
                exit(0);
            }
            break;
            default: exit(0);
        }
    } //while menu;
        
}