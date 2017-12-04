#include <stdio.h>
#define MAX_ROW 100

int main(void){
  printf("Enter Rows: ");
  int rows,s=0; //s->counter
  scanf("%d", &rows);
  int num[rows][MAX_ROW]; //max length of row is MAX_ROW.
  int len[rows], l = rows; //dynamically get length of each row
  while(l ) { //set everything = 0
    len[l-1] = 0;
    s=0;
    while(s++ < 100){
      num[l-1][s] = 0;
    }
    // printf("Set0: %d\n", l); //debug
    l--;
  }

  l = rows;
  while(l){
    printf("Row No. %d: Enter Length (<=100)->\t", l);
    scanf("%d", &len[l-1]);
    if(len[l-1] > MAX_ROW) {
      printf("Enter Row size less than 100.\n");
      return 0;
    }
    for(int i=0; i<len[l-1] ; i++) {

      if(!scanf("%d", &num[l-1][i])) i = len[l-1]+1; //to break out of loop

    }
    l--;
  }
  printf("\n::SPIRAL::\n");
  // Print all elements..
  /*
  for(int i=0; i<rows; i++){
    printf("\nLength of row %d = %d\n",i,len[i]);
    for(int j =0; j<len[i]; j++){
      printf("%d\t(i,j):%d,%d\t\n", num[i][j],i,j);
    }
  }
  */
  for(int i=0; i<rows; i++){
    if(i%2==0){
      for(int j=0; j<len[i]; j++){
        printf("%d ", num[i][j]);

      }
    }
    else {
      for(int j=(len[i]-1); j>=0; j--){
          printf("%d ", num[i][j]);
        }
      } //else
    printf("\n");
    } //for


  return 0;
} //main
