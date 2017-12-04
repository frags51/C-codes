#include <stdio.h>
size_t strlen(const char c[]);

void strrev(char c[]){
  char temp[strlen(c)+1];
  for(int i=0; i<strlen(c); i++) {
    temp[i] = c[strlen(c) - i -1];
  }
  temp[strlen(c)] = '\0';
  for(int i=0; i<strlen(c); i++) c[i] = temp[i];
  //c=temp; //wrong - use pointers to make it correct.
}

int main(){
  char c[101];
  printf("Enter a string (max 100): ");
  scanf("%s", c);
  strrev(c);
  printf("%s\n", c);
  return 0;
}
