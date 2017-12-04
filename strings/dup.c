#include <stdio.h>
size_t strlen(const char c[]);
void strDel(char *c, const int index);
void dupDel(char *c);

int main(){
  char c[101];
  printf("Enter Less than 100 chars to delete duplicates..: ");
  scanf("%s", c);
  dupDel(c);
  printf("\n%s\n", c);
  return 0;
}

size_t strlen(const char c[]){
  size_t i;
  for(i = 0; c[i]!='\0'; i++);
  return i;
}

void strDel(char *c, const int index){
    if (index > (strlen(c) -1)) {printf("Error!\n");return;}
    c+= index;
    for(; *c != '\0';c++)
        *c = *(c+1);
}

void dupDel(char *c){
  int i, j;
  //char *dups;
  for(i=1; c[i]!='\0'; ++i){
    for(j=0; j<i;j++){
      if(c[j]==c[i]) break;
    }//for
    if(i-j) {
      strDel(c, i);
      i-=1;
    }//if

  }//for
}
