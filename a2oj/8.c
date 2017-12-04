#include <stdio.h>

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

int main(void){

  char a[101], b[101];
  scanf("%s %s", a, b);

  for(int i = 0; a[i]!='\0'; i++){
    if(a[i]==b[i]){
      putchar('0');

    }
    else putchar('1');
  }
  return 0;
}
