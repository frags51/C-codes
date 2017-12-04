#include <stdio.h>
int abs(int x){
  return x>0 ? x : -x;
}

int main(void){
  int a[5][5];
  int i = 0, j=0, r,c;
  while(i++ < 5)
  {
    j=0;
    while(j++ <5) {
    scanf("%d", &a[i-1][j-1]);
    if(a[i-1][j-1] ==1) {
      r= i-1;
      c = j-1;
    }

  }

  }

  printf("%d\n", abs(r-2) + abs(c-2));
  return 0;

}
