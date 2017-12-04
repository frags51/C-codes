/*
cs16btech11038
Assignment 2: part A
*/
#include <stdio.h>

int numRev(int num, int * digits);

int main(void){
  int num, digits =0;
  printf("Please enter the number you want to reverse: ");
  if(scanf("%d", &num)){ //to check if the input was correct(only a number)
    int x = numRev(num, &digits);
    printf("Reversed number is: %.*d\n", digits, x); //.* : For ints, the min no of "digits" to be printed ; %* : min chars to be printed
/*
//    Alternate code:
    if(num >= 0) printf("Reversed number is: %0*d\n", digits, x);
    else printf("Reversed number is: %0*d\n", digits+1, x); // '-' eats up 1 width.
*/

  } // IF

  else printf("Please enter a valid number.\n");

  return 0;
} //main

int numRev(int num, int * digits){
  int x = 0;
  do {
    x += num % 10; //last digit of num
    num /= 10; //divide the no by 10 to change the last digit :: num is int
    x *= 10;
    (*digits)++;
  } while(num); //as long as g.int (num) != 0

  return x/10;   //an extra 10 is multiplied to x.
} //numRev
