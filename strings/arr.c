#include <stdio.h>
int max2(int n1, int n2){
  return n1>n2?n1:n2;
}
int max3(int n1, int n2, int n3){
  return n1 > n2 ? (n1 > n3 ? n1 : n3) : (n2 > n3 ? n2 : n3);
}
int maxA(int a[], const int len){ //find max element in array of ints...
  int mA=0;
  for(int i=0; i < len -1; i = i+1){
    if(i==0) mA = max2(a[i], a[i+1]);

    else mA = max3(a[i], a[i+1], mA);
  }
  return mA;
}
int main(){
    int arr[6][6];
    for(int arr_i = 0; arr_i < 6; arr_i++){ //scanning input
       for(int arr_j = 0; arr_j < 6; arr_j++){
          scanf("%d",&arr[arr_i][arr_j]);

       }
    }
    int i,j,k;
    i=j=k=0;
    int hSum[16];
    for(k=0;k<4;k++){
      for (j=0;j<4;j++){ // important to set j= 0 every time the loop iterates.
        hSum[i]= arr[k][j] + arr[k][j+1] + arr[k][j+2] + arr[k+1][j+1] + arr[k+2][j] + arr[k+2][j+1] + arr[k+2][j+2];
      //printf("Debug(hSum[i],j,k,i): %d\t%d\t%d\t%d\n", hSum[i], j, k,i);
        i++;
      }
      //printf("%d\n", k);
    }
    printf("%d\n", maxA(hSum, 16));
    return 0;
}
