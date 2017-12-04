#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
//for INT_MAX
#define BASE 2
// base of the log.
void welcome(void);
int validate(long int *a,long int *b);
long int add(long int a,long int b);
long int mult(long int a,long int b); //mult of ints may result in long int answers.
long int Div(long int a,long int b);
long int sub(long int a,long int b);
long int mod(long int a,long int b);
long int sqr(long int a);
long int sqrt(long int a);
long int log(long int a);
//functions
int main(void){
  system("clear"); //in stdlib
  printf("CALCULATOR\nBuilt by Supreet Singh CS16BTECH11038\n");

  welcome();

  return 0;
} //main

void welcome(void){ //this function is called again and again.
  printf("\n");
  printf("MENU: \nEnter the number corresponding to the operation you want to perform. \n");
  printf("1: Add\n2: Multiply\n3: Divide\n4: Subtract\n5: Modulus\n6: Square\n");
  printf("7: Square Root\n8: Log (Base : %d)\n9: Exit\n", BASE);
  printf("Your Choice: ");

  int ch = 0; //for choice
  //initally i added this to check overflows, but now it checks for "string" type errors.
  static int FLAG = 0; //First instance of welcome: No need to check for overflows.
  char c;
  if(FLAG !=0 ){
    while((c=getchar()) !='\n'); //if overflowed, scan unnessary numbers/characters.
  } //ALSO takes care of character "asfa" etc inputs.
  FLAG =1; //after first instance we need to check.

// NOW IT DOESN'T Check for overflows. It corrects "string" type inputs. else forever loop.

  if(!scanf("%d", &ch) || ch <1 || ch >9) {//scan at max 10 digits(as INT_MAX has 10 digits)
    printf("ERROR: Enter a valid number(1-9)\n");
    welcome();
  } //some validation
  else{ //if valid data
    long int a=0,b=0; //long ints taken for validation of int size.
    switch(ch){ //User's choice menu
      case 1: //though a, b are long ints by def, we have checked they are <=INT_MAX hence can be passed as int arguments.
      if(validate(&a, &b)) printf("The result is: %ld\n", add(a, b));
      welcome();
      break;

      case 2:
      if(validate(&a, &b)) printf("The result is: %ld\n", mult(a, b));
      welcome();
      break;

      case 3:
      if(validate(&a, &b) && b!=0) printf("The result is: %ld\n", Div(a, b)); //div already exists in stdlib.
      else if(a>=0 && b==0) {
        printf("2nd number must be positive.\n");
        welcome();
      }
      welcome();
      break;

      case 4:
      if(validate(&a, &b)) printf("The result is: %ld\n", sub(a, b));
      welcome();
      break;

      case 5:
      if(validate(&a, &b) && b!=0) printf("The result is: %ld\n", mod(a, b));
      else if(a>=0 && b==0) {
        printf("2nd number must be positive.\n");
        welcome();
      }
      welcome();
      break;

      case 6:
      if(validate(&a, NULL)) printf("The result is: %ld\n", sqr(a));
      welcome();
      break;

      case 7:
      if(validate(&a, NULL)) printf("The result is: %ld\n", sqrt(a));
      welcome();
      break;

      case 8:
      if(validate(&a, NULL) && a!=0) printf("The result is: %ld\n", log(a));
      else if(a==0) {
        printf("Number must be positive.\n");
        welcome();
      }
      welcome();
      break;

      case 9:
      printf("Thanks for using the calculator!\nBye!\n");
      exit(0);
      break;
    } //switch *ch
  } // else -- after validation

} //welcome

int validate(long int *a,long int *b){
  if(b != NULL){
    printf("Enter the first number: ");
    if(!scanf("%ld", a) || *a<0 || *a>INT_MAX){

      printf("Enter positive number and within range of int: <= %d \n", INT_MAX);
      return 0;
    } //scanf a if
    else{
      printf("Enter second number: ");
//      char c;
//      while((c=getchar()) !='\n'); //To prevent overflow related errors as done above.
      if(!scanf("%ld", b) || *b<0 || *b>INT_MAX){
        printf("Enter positive number and within range of int: <= %d \n", INT_MAX);
        return 0;
      } //scanf b if
      else return 1;
    }//scanf a else
  } //b = NULL if2
  else if(b==NULL){
    printf("Enter the number: ");
    if(!scanf("%ld", a) || *a<0 || *a > INT_MAX){
      printf("Enter Valid Number (non-negative and within range of int: <= %d)\n", INT_MAX); //Limit the input
      return 0;
    }

  }//else if b==NULL
  return 1;
} //validate

long int add(long int a,long int b){
  return a+b;
} //add

long int sub(long int a,long int b){
  return a-b;
} //sub

long int mult(long int a,long int b){ //add a , 'b' times..
  if(b>0){
    long int c=a;
    for(int i=1 ;i<b; i++) c+=a;

    return c;
  }
  else return 0; // if(b==0)
} //mult

long int Div(long int a,long int b){ //keep subracting b from a till a>=0
  int i = 0; //we have already validated data.
  for(; a>0; i++){
    a-=b;
  }

  if(a<0) return (long int) i-1;
  else return (long int) i;
} //Div

long int mod(long int a,long int b){
  return a- ((Div(a,b)) * b);
} //mod

long int sqr(long int a){
  return mult(a, a);
}//sqr

long int sqrt(long int a){
  int i=0;
  for(;mult(i, i) <= a; i++);
  return i-1;
} //sqrt

long int log(long int a){
  int i = 0;
  long int c=1;
  for(;a>= c;  ){
    c=mult(c, BASE);
    i++;
  }

  if(a==Div(c,BASE)) return i-1; //if result is an int..
  else return i; //ceil...
} //log
