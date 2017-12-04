#include <stdio.h>
typedef struct{
  int n;
  int a;
} matrix;

int main(void){
  printf("Enter Order Of The Matrix: ");
  int n=0;
  scanf("%d", &n);
  int matrix[n+1][n+1];
  printf("Enter the matrix NxN: \n");
  for(int i=1; i<n+1;i++) for(int j=1;j<n+1;j++) scanf("%d", &matrix[i][j]);


  return 0;
}//main
