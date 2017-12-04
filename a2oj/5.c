#include <stdio.h>

int main(void){
  int year;
  scanf("%d", &year);
  int d[4];
  year++;
l:  d[0] = year%10;
  d[1]=(year/10)%10;
  d[2]=(year/100)%10;
  d[3]=(year/1000)%10;
  for(int i=0; i<3;i++){
    for(int j=i+1;j<4;j++)
    if(d[i]==d[j]) {
      year++;
      goto l;
    }
  }

  printf("%d", year);
  return 0;
}
