#include <stdio.h>

int fact(int a){
  if (a >= 2) {
    a= a*fact(a-1);
  }
  else return 1;
  return a;
}

int main(){
  int a;
  printf("Enter an integer: ");
  scanf("%d", &a);
  printf("Factorial is: %d\n", fact(a));
  return 0;
}
