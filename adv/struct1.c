#include <stdio.h>

int main(void){
  struct point {
    int x;
    int y;
  } ;
  struct point x = {1 , 2};
  printf("%d\n", x.y);
  return 0;
}
