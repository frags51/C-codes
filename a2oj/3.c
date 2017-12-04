#include <stdio.h>
void swap(char *s, int i, int j){
  char temp;
  temp = s[j];
  s[j] = s[i];
  s[i] = temp;
}

int main(void){
  int n, t;
  scanf("%d %d", &n ,&t); // t : the no of swaps..
  char s[n+1]; //for \0 char
  scanf("%s", s);
  int index;
  while(t--){ //evaluate t, then decrements it immediately...
    for(index=0;s[index+1]!='\0';) //index initialization inside the for loop to change it to 0 for every t
    {
    if (s[index]=='B' && s[index+1]=='G') {
      swap(s, index, index+1);
      index+=2;
    }
    else index++;
  } //for
  } //wwhile
  printf("%s\n", s);
  return 0;
}
