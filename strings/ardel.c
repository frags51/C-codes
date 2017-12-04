#include <stdio.h>

size_t strlen(const char c[]); //defined in "strlen.c" or <strings.h>

void strDel(char *c, const int index){
    if (index > (strlen(c) -1)) {printf("Error!\n");return;}
    c+= index;
    for(; *c != '\0';c++) *c = *(c+1);
}

int main(){
  char c[999];
  int index =0;
  printf("Enter as string + index ");
  if(scanf("%s + %d", c, &index) == 2){
    strDel(c, index);
    printf("\n%s\n", c);
    return 0;
  }
  else {
    printf("Enter The format correctly!\n");
    return 0;
  }
}
