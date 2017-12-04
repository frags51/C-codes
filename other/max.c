#include <stdio.h>

int main(){
  int a[3], max;
  printf("Enter 3 numbers(Num1 Num2 Num3): ");
  scanf("%d %d %d", &a[0], &a[1], &a[2]);
  max = a[0] > a[1] ? (a[0] > a[2] ? a[0] : a[2]) : (a[1] > a[2] ? a[1] : a[2]);
  printf("%d", max);
  return 0;
}
