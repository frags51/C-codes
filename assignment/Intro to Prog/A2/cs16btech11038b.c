/*
cs16btech11038
Assignment 2: Part B
*/

#include <stdio.h>
#define MAX_PRIME 64
//max no of prime factors (distinct) of an int< 2 ^64
//probably lesser than 32.. just take 64 to avoid error
void fact(long int num, int * arr){ //num > 1

  int i, j, index = 0;
  for(i=2; i<=num; i++){
    if(num % i == 0) {   // first check if i is a factor of num, then if is prime.
      for(j = 2; (j*j <= i && i>3) || (j<=i && i <=3); j++){ //to incl 2 & 3
        if(i % j == 0 && i!=2){ //2%2 ==0 but 2 is prime
          break;
        } // if: NOT PRIME i
        else if((i%j != 0 && (j+1)*(j+1) > i) || (i==2))
        {
//        printf("%d\n", i); //for debugging purpose
          arr[index++] = i;
        } //if: PRIME(i)

      } //second for

      //add
      num/=i; //To decrease the calculation time, just divide by the factors.
      //
    } // if
  } //first for
} // fact

int main(void){
  printf("Enter the number: ");
  long int num;
  scanf("%ld", &num);
  if(num<2) {
    printf("Input a number >= 2\n");
    return 0;
  } //if
  int prime_facts[MAX_PRIME];
  for(int i = 0; i<MAX_PRIME; i++) prime_facts[i] = 0; //set all array elements to 0
  fact(num, prime_facts);
  int i = 0;
  printf("The prime factor(s) are:");
  while(1) { //to print the output...
    if(prime_facts[i+1] && prime_facts[i++]) {
      printf(" %d,", prime_facts[i-1]); //since i has been incremented above
    }
    else if(!prime_facts[i+1] && prime_facts[i++]){
      printf(" %d\n", prime_facts[i-1]);
      break;
    }
  } //while

  return 0;
} //main
