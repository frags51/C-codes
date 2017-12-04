#include <stdio.h>
#include <stdlib.h>
void strl(char *s)
{
  for(;*s;s++)
  {
    if(*s < 'a' && *s >= 'A') *s+= 'a' - 'A';
  }
}
void stru(char *s)
{
  for(;*s;s++)
  {
    if(*s > 'Z' && *s <= 'z') *s+= 'A' - 'a';
  }
}
int main(void)
{
//  char *s = (char*)malloc(101*sizeof(char));

//  char *p = s;
  char p[101];
  scanf("%s", p);
  int u_c = 0, l_c = 0;
  int i=0;
  while(p[i])
  {
    if(p[i] > 'Z' && p[i] <='z') l_c++;
    else if(p[i]<'a' && p[i] >= 'A') u_c++;
    i++;
  }
  l_c >= u_c ? strl(p) : stru(p);
  printf("%s\n", p);
//  free(s);
  return 0;
}
