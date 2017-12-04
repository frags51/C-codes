#include <stdio.h>

int main(void){
 int n;
  scanf("%d", &n);
  int x[n], y[n], z[n];
  int temp = n;
  while(n--){ // checks n, then decrements it. So in scanf, we get n-1 in the first turn..
    scanf("%d %d %d", &x[n], &y[n], &z[n]);
  }
  int i,a=0,b=0,c=0;

  for(i=0; i<temp; i++) a+=x[i];
  for(i=0; i<temp; i++) b+=y[i];
  for(i=0; i<temp; i++) c+=z[i];
  if(a==0 && b==0 && c==0) printf("YES");
  else printf("NO");
  return 0;
}
