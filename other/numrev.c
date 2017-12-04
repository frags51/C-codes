#include <stdio.h>
int numrev(int a){
  int rem, rev=0;
  while(a) {
    rem = a%10;
    rev*=10;
    rev+=rem;
    a/=10;
  }
  return rev;
}
int main(){
  int a=0;
  printf("Enter a number to reverse: ");
  scanf("%d", &a);
  printf("%d\n", numrev(a));
  return 0;
}
