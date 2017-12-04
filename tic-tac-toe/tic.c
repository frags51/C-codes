#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CROSS 1
#define CIRCLE 2
char user_ch = 0;
char cpu_ch = 0;
char p_area[3][3];

void print_ar(){
  for(int i=0; i<3; i++) {
    for(int j=0; j<3; j++) p_area[i][j]==0? printf(" - ") : printf(" %c ", p_area[i][j]);
    putchar('\n');
  }
} //print_ar
int u_move(void);
int cpu_move(void);
int main(void){
  printf("Choose: X / O: ");
  if(!scanf("%c", &user_ch) || (user_ch!='X' && user_ch!='O')){
    printf("Please enter correct choice!\n");
    exit(1);
  }
  switch(user_ch){
    case 'X': cpu_ch = 'O'; break;
    case 'O': cpu_ch = 'X'; break;
    default: printf("Error! Wrong choice\n"); exit(1); break;
  }

  u_move();
  return 0;
} //main

int u_move(void){
  int t;
  printf("Enter the tile you want to place your mark at: ");
  do{
    scanf("%d", &t);
    if(t>=1 && t<=9 && p_area[(int)(t-1)/3][(t-1)%3]==0) break;
    else{
      printf("ERROR! Please enter a valid tile number (1-9)\n");
      continue;
    }
  }while(1);
  p_area[(int)(t-1)/3][(t-1)%3]=user_ch;
  printf("--Your Turn Ended--\n");
  print_ar();
  return t; //returns the tile number user entered.
}//u_move

int cpu_move(void){
  
}
