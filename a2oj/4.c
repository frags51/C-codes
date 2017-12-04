#include <stdio.h>

int main(void){
  char ch;
  while((ch = getchar()) != '\n' ){
    if(ch=='.') putchar('0');
    else if (ch == '-') {
      ch = getchar();
      if(ch  == '.') putchar('1');
      else if(ch == '-') putchar('2');
    }
  }

  return 0;
}
