#include <stdio.h>

int main(void)
{
  char c[20];
  fgets(c, 20, stdin);
  fputs(c, stdout);
  fputs(" hellO!\n", stdout);
  fgets(c, 20, stdin);
  fputs(c, stdout);
  fputs("\n 22: hellO!\n", stdout);
  return 0;
}
