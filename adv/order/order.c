#include <stdio.h>
#define ITEMS 5

char *oNames[] ={"Not Valid Order" "Name 1", "Name 2", "Nmame 3", "Name 4", "Name 5" };
//typedef enum {noItem, T1, T2, T3, T4, T5} orders;
typedef struct{
  int a;
} order;
int getOrder(void)
{
  printf("What would you like to order??\n")
  for(int i= 1; i<ITEMS+1; i++)
  {
    printf("\nFor %s, input: %d\n", oNames[i], i);
  }
  int c;
  scanf("%d", &c);
  return c;
} //getOrder

int main(void)
{
  getOrder();

  return 0;
}
