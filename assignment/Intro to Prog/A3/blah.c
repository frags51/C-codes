#include <stdio.h>

int months[13] = {0, 31, 28, 31, 30,31,30,31,31,30,31,30,31};
int cur_day(int m, int y){ // gives number corresponding to the day on 01/m/y :: 00 is Sunday.
  int is_leap = ((y%4 == 0 && y%100!=0) || y%400 == 0) ? 1 : 0;
  int days = 0; //no of days or days%7 passed since 1/1/1900
  int leap_years_passed = 0;
  for(int i = 1900; i< y; i++){
    if(((i%4 == 0 && i%100!=0) || i%400 == 0)) leap_years_passed++;
  }
  //int leap_years_passed = (int) f;
  days+= y-1900; //365%7 = 1
  printf("first - %d\n", days);
  printf("ly-passed:: %d -- \n", leap_years_passed);
  days+= (leap_years_passed%7);
  printf("l-y-p: %d\n", days);
  //reached 01/01/year
  for(int i = 1; i<m; i++){
    days += months[i];
  }//for : To add days passed till previous month.
  printf("m-i : %d\n", days);
  if(m>2 && is_leap) {days+= 1; printf("BSFA\n");}
  days%=7;
  printf("mod - %d\n", days);
  days+=1; //to MAKE SUNDAY == 0
  printf("-sun - %d\n", days);
  //if(!is_leap) days++; //
  days%=7;
  printf("final - %d\n", days);
  return days;
}

int main(){
  int x =0;
  while(1){scanf("%d", &x); printf("%d\n", cur_day(1, x));}
}
