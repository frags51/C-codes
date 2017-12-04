#include <stdio.h>
#include <string.h>

int main(void){
  printf("Enter 2 stinrs: ");
  char a[100], b[100];
  scanf("%s %s", a, b);
  printf("Answer is: %d\n", strcmp(a, b));
  return 0;
}
