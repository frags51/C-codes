#include <stdio.h>

int main(void){
  int n;
  scanf("%d", &n);
  char s[n+1];
  scanf("%s", s);
  int cnt;
  for(int i= 0; i<n;i++){

    for(int j=i+1; s[j]; j++){
      
      if(s[j] == s[i]){
        cnt++;
      }
      else{
        i = j;
        break;
      }


    }


  }
  printf("%d", cnt);
  return 0;
}
