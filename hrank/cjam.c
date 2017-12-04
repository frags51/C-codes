#include <stdio.h>
#include <math.h>
#define DEBUG 1
int main(void){
    int t;
    scanf("%d", &t);
    for(int t_count = 1; t_count < t+1; t_count++){
        long long int num;
        scanf("%lld", &num);
        int loop_counter=0;
        while(1){
            
            long long int dummy = num;
            int swaps = 0, d_count = 0, d_equals = 0;
            while(1){
                int fl=0;
                if(DEBUG) printf("\tNum :%lld, Dummy: %lld\n", num, dummy);
                if(dummy <= 0) break;
                long long int dummy2 = dummy;
                while(dummy2 > 9){
                    if(dummy2%10 < (dummy2/10) % 10) {fl=1; break;}
                    dummy2/=10;
                } //if(dummy%10 < (dummy/10) % 10)
                if(fl) {
                    //num+=9*pow(10, d_count);
                    //num-=(dummy%10);
                    do {
                        //if(DEBUG) printf("      NUM==== %lld\n", num);
                        num = num - (long long int) pow(10, d_count-d_equals)*(((long long int)(num / (long long int)(pow(10, d_count-d_equals))))%10);
                        if(DEBUG) printf(" equals: %d: , count: %d  --num%%10 power dC- dE: %lld-- \n", d_equals,d_count,num);
                        num = num + (long long int)9*pow(10, d_count-d_equals);
                        if(DEBUG) printf("    --+9*blah: %lld-- \n", num);
                        d_equals--; 
                    }while(d_equals > -1);
                    num-=(pow(10, d_count+1));
                    swaps++;
                    d_equals = 0;
                    if(DEBUG) printf("    >>>changed @pos %d<<<   ", d_count);
                }
                else if(dummy%10 == (dummy/10) % 10){
                    d_equals++;
                }
                else d_equals = 0;
                d_count++;
                dummy=num/pow(10, d_count); //decrement.
                
            } //inner loop
            if(DEBUG) printf("  \t\tSWAPS: %lld", swaps);
            loop_counter++;
            if(swaps == 0 || loop_counter > 19) break;
            
        } //outer loop
        printf("Case #%d: %lld\n", t_count, num);
    } //t_count
}//main