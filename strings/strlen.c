#include <stdio.h> //size_t is defined in stdio.h === unsigned long int
size_t strlen(const char c[]){
  size_t i;
  for(i = 0; c[i]!='\0'; i++);
  return i;
}
int strrindex(const char c[], const char t[]){
  int j,k;
  for(int i=0; c[i]!='\0';++i){
      for(j=i, k=0; t[k]!='\0' && t[k] == c[j]; j++, k++);
      if(t[k]=='\0') return strlen(c) - i - strlen(t);
  }
  return -1;
}
