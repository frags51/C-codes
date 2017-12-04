/*
cs16btech11038
Assignment 1: Part C
*/

#include <stdio.h>
int main(void){
  int d, m ,y;
  printf("Enter the day: ");
  scanf("%d", &d);
  printf("Enter the month: ");
  scanf("%d", &m);
  printf("Enter the year: ");
  scanf("%d", &y);
  if(d<1 || m < 1 || y < 1) {
    printf("Enter positive values only.\n");
    return 0;
  } // Validator 1
  int is_leap = ((y%4 == 0 && y%100!=0) || y%400 == 0) ? 1 : 0;
  int months[13] = {0, 31, 28, 31, 30,31,30,31,31,30,31,30,31}; //13 size for simplicity
  if(m>12 || ((!is_leap || m!=2) && months[m]<d) || (is_leap && m==2 && d>29)){
    printf("Enter valid dates\n");
    return 0;
  } //Validator 2
  int days = 0; //no of days or days%7 passed since 1/1/1

  int leap_years_passed = (y-1)/4 + (y-1)/400 -(y-1)/100;
  days+= y-1; //365%7 = 1
  days+= (leap_years_passed%7);
  //reached 01/01/year
  for(int i = 1; i<m; i++){
    days += months[i];
  }//for : To add days passed till previous month.

  if(m>2 && is_leap) days+= 1;
  days+= (d-1);
  days%=7;

  switch(days) { //Print the day according to the remainder mod 7
    case 0: printf("Monday\n");
    break;
    case 1: printf("Tuesday\n");
    break;
    case 2: printf("Wednesday\n");
    break;
    case 3: printf("Thursday\n");
    break;
    case 4: printf("Friday\n");
    break;
    case 5: printf("Saturday\n");
    break;
    case 6: printf("Sunday\n");
    break;
    default: printf("Not existant\n");
    break;
  } //switch
  return 0;
} //main
