/* Calculate bezout's coefficients of two no a and b : sa + tb = gcd(a,b) */

#include <stdio.h>
unsigned int gcd(const int a,const int b){
  if(a<0 || b<0) return 0;
  if (a*b==0) return a>b ? a : b;
  else return gcd(a>b?b:a, (a>b ? a : b) % (a>b?b:a)); //Euclid's Algorithm
}

int main(void){
  int a=0, b=0;
  printf("Enter Two Numbers (a b): ");
  scanf("%d %d", &a, &b);
  int d = gcd(a,b);
  if(d==0) goto label;
  a /= d;
  b /= d;
  int s;
  for(s = 0;;s++){
    if((s*a)%b == 1) break;
  }
  printf("GCD is: %d\n", d);
  printf("The coeffs s and t are: %d %d\n", d*s, d*((1-s*a)/b));
  return 0;
label: printf("Enter Positive Ints..\n");
  return 0;
}
