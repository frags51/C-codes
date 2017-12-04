#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main (void){
  int a[3][3]={1,2,3,4,5,6,7,8,9};

  int * p[] = {a, a+1, a+2};
//  int (*var)[3];
//  var =a ;
//    printf("*P: %p\n*P+1 %p\n", *p, *(p+1));
//  printf("*a : %d\n", *(a));

// a and var are compatible - no compiler warning --> a is a pointer to array[3] of integers, not pointer to int (cdecl)
// a is being typecasted on declaring the int* p[] ??

//*(p+i) == a+i (pointers *(p+i) and a+i may not be of the same type, but they point to the same address)
//see result of printf 3)

//in printf 1), using *(p+i) { == a+i} or using *(a+i) gives same result.. why? (above point)
  printf("---------\n");
  for(int i=0; i<3; i++)
  {
    for(int j=0; j<3;j++)
    {
      //1)
      printf(">%d %d  ===   %d %d\n", *(*(p+i)+j), *(*(p+j) + i),*(*(a+i)+j), *(*(a+j) + i) );
      
      //2) address of a[0] and a[0][0] is same. But a[0] has its address as data and a[0][0] has 1 stored as data
      //how can same address have different data stored??
      printf("a+i: %p, &a[i]: %p, a[i] : %p, &a[i][0]: %p, a[0][0]: %p\n", a+i, &a[i], a[i], &a[i][0], a[0][0]);
      //3)
      printf("--*(p+i): %p, a+i: %p, i: %d, *(p+i) == (a+i)? Ans: %d\n", *(p+i),a+i, i, *(p+i) == (a+i));
      //4)
//      printf("*(a+i):%d, *(a+i+j): %d, *(*(p+i) + j) %d\n", *(a+i), *(a+i+j), *(*(p+i) + j));
      printf("\n");
    }

  }
  return 0;
}
