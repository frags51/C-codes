#include <stdio.h>
#include <math.h>
#define MAX(a, b) (a>b?a:b)
#define MIN(a, b) (a<b?a:b)


int funny(int n);
int c=0;
int binarysearch_prev(int *a,int m,int n)
{

    int l,u,mid;
l=0,u=n-1;
    while(l<=u)
        {
         //printf("\tl %d, u %d, mid : %d", l, u, mid);
         mid=(l+u)/2;
         if(m==a[mid])
            {
             c=1;
             break;
         }
         else if(m<a[mid])
            {
             if(u!=l && u!=mid) u=mid;
             else if(u!=l && u == mid) u = mid-1;
             else if(u==l) {/*printf("\tl %d, u %d", l, u);*/return l-1; } //prev closest.
         }
         else {
             if(u!=l && l!=mid) l=mid;
             else if(u!=l && l==mid) l=mid+1;
             else if(u==l) {/*printf("\tl %d, u %d", l, u);*/ return l;} //prev closest
           }
    }
    return mid;
}
int binarysearch_next(int *a,int m,int n)
{

    int l,u,mid;
l=0,u=n-1;
    while(l<=u)
        {
         mid=(l+u)/2;
         if(m==a[mid])
            {
             c=1;
             break;
         }
         else if(m<a[mid])
            {
             if(u!=l && u!=mid) {u=mid; }
             else if(u!=l && u==mid){u=mid-1;}
             else if(u==l) {return l; } //next closest.
         }
         else {
             if(u!=l && l!=mid) {l=mid; }
             else if(u!=l && l==mid){l = mid+1;}
             else if(u==l) {return l+1; } //next closest
           }
    }
    return mid;
}

int is_fun(int n){
  while(n){
    if(n%10==4 || n%10==7) continue;
    else return 0;
    n/=10;
  }
  return 1;
}//is_fun

int main(void){
  int a,b,c,d,k;
  int pairs = 0;
  scanf("%d%d%d%d%d", &a, &b, &c, &d, &k);

  int fun_no[funny(MAX(b,d))]; int fun_no_ptr = 0;

  for(int t=3; t<=MAX(b,d); t++){
    if(is_fun(t)) fun_no[fun_no_ptr++] = t;
  } //create an array of funny numbers upto MAX(b, d)
  int fun_len = fun_no_ptr-1;
  fun_no_ptr = 0;

  //

  for(int i=a; i<b+1; i++){
    for(int j=c; j<d+1; j++){
      int min, max;
      if(fabs(i-j)+1 < k) continue; //no sense

      else if(fabs(funny(i) - funny(j)) == k){
        if(i < j){
          int star= binarysearch_next(fun_no, j, fun_len);
          if(fun_no[star] <=d ){
            int star2 = binarysearch_next(fun_no, i, fun_len);

            if(fun_no[star2] <=c) {pairs+= ((fun_no[star2]-i+1) * (MAX(fun_no[star]-j,1))); i+=(fun_no[star2] - i); break;}
            else if(fun_no[star2] > c) {pairs+= (c - i + 1) * (MAX(fun_no[star]-j, 1)); i+=(c - i + 1); break;}
          }
          else if(fun_no[star] > d){
            int star2 = binarysearch_next(fun_no, i, fun_len);
            if(fun_no[star2] <=c) {pairs+= ((fun_no[star2]-i+1) * (d-j+1));i+=(fun_no[star2] - i); break;}
            else if(fun_no[star2] > c) {pairs+= (c - i + 1) * (d-j+1); i+=(c - i + 1); break; }

          }
        } //i<j
        else{
          int star= binarysearch_next(fun_no, i, fun_len);
          if(fun_no[star] <=c ){
            int star2 = binarysearch_next(fun_no, j, fun_len);

            if(fun_no[star2] <=d) {pairs+= ((fun_no[star2]-j+1) * (MAX(fun_no[star]-i,1))); j+=(fun_no[star2] - j); break;}
            else if(fun_no[star2] > d) {pairs+= (d - j + 1) * (MAX(fun_no[star]-i, 1)); j+=(d - j + 1); break;}
          }
          else if(fun_no[star] > c){
            int star2 = binarysearch_next(fun_no, j, fun_len);
            if(fun_no[star2] <=d) {pairs+= ((fun_no[star2]-j+1) * (c-i+1));j+=(fun_no[star2] - j); break;}
            else if(fun_no[star2] > d) {pairs+= (d - j + 1) * (c-i+1); j+=(d - j + 1); break; }

          }
        } //j<=i
      } //else

    }//for
  }//for
/*  int r;
  scanf("%d", &r);
  printf("--> %d\n", funny(r));
*/
  printf("%.12lf",(double) pairs/((b-a +1) * (d-c+1)));
  return 0;
}

int funny(int n){ //counts funny numbers upto n
  int count=0;
  int digits = (int) log10(n) + 1;
  for(int i=1; i< digits; i++){
    count+= (int) pow(2, i);
  } // funny no upto digits-1 digits..
  int a, b;
  for(int i=digits-1;i>-1 ; i--){
    b = pow(10, i);
    a = n / b;
    if(a < 4) {
      break;
    }
    else if(a==4){
      if(i==0) {count++; break;}
      else {n%=b; continue;}
    }
    else if(a>4 && a<7){
      if(i==0) {count++; break;}
      else {count+= pow(2, i);break;}
    }
    else if(a==7){
      if(i==0) {count+=2; break;}
      else {count+=pow(2, i); n%=b; continue;}
    }
    else if(a > 7){
      if(i==0) {count+=2; break;}
      else {count+= 2*pow(2, i);break;}
    }
  } // for
  return count;
}//funny
