#include <stdio.h>
void filecopy(FILE * ifp, FILE * ofp)
{
  char c;
  while((c=getc(ifp)) != EOF)
  {
    putc(c, ofp);
  }

}

int main(int argc, char *argv[])
{
  if(argc == 1)
  {
    filecopy(stdin, stdout);
    return 0;
  }
  else if(argc > 1)
  {
    FILE *fp = NULL;
    --argc;
    for(;argc;--argc)
    {
      if((fp = fopen(*++argv, "r")) == NULL)
      {
        printf("ERROR!\n");
        return 1;
      }
      else
      {
        filecopy(fp, stdout);
        fclose(fp);
      }
    }
  }
  return 0;
} //main
