#include <stdio.h>
#include "strindex_2.c"
#include "strlen.c"
//design strindex(s,t) -> position in string s where t begins
int strindex(const char s[], const char t[]){ //int or long int??
  int i,dex=0,k =0;
  short int j=0;
  for(i = 0; s[i]!='\0'; i++){
      if( (t[k]==s[i]) && (t[k]!='\0') ){
        j = 1;
        if(k==0) { dex = i; /*printf("Good\n\tdex:%d\n\tj:%hd\n\tk:%d\n\ti:%d\n", dex, j, k,i);*/}
        k++;

      }
      else if(t[k]!='\0' &&  (t[k]!=s[i]) ) {/*printf("Badx\n\tj:%hd\n\tdex:%d\n\tk:%d\n\ti:%d\n", j, dex, k, i);*/if((t[k-1]==s[i-1]) && k>0 && i>0) i=i-1;j=dex=k=0; }
      else if(t[k]=='\0') {/*printf("Ender?\nj:%hd\ndex:%d\nk:%d\n", j, dex, k);*/ break;} // for break
  }//for/
  switch(j){
    case 1: return dex;
            break;
    case 0: return -1;
            break;
    default:return -2;
            break;
  } //switch
} //strindex

int main(){
  char s[10000], t[10000];
  printf("ENTER A String:\n");
  scanf("%s", s);
  printf("ENTER A PATTERN to search for:\n");
  scanf("%s", t);
  printf("S:%s\nT:%s\n", s, t);
  printf("\nLindex:%d\nLindex:%d\nRindex:%d\nStrLen:%lu\n", strindex(s, t), strindex2(s,t), strrindex(s,t), strlen(s));
  return 0;

} //main
