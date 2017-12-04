#include <stdio.h>
#include <math.h>
int *primes(int n, int *len);
int main(){
  int a=0;
  int *b = primes(14, &a);
  printf("%d\n", *b);
  return 0;

}

int *primes(int n, int *len){
  int i,j;
  int *p, *q;
  *p = 2;
  q = p;
  *(p+1) = 3;
  p+=1;
  for(i=2; i<n; i++){
    for(j=2; j<=(int)sqrt(i) && (i%j!=0);j++);
    if (j>(int)sqrt(i)) *++p = i;

  }
  *len = p-q;
  return q;
}
