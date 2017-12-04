#include <stdio.h>
#include <stdlib.h>
//for exit(), system();
void menu(void);
void d_year(int y);
void d_month(int m, int y);
void right(int);
void left(int);
int m=1, y=0; //global vars, since they need to be accessed by all the functions.
int months[13] = {0, 31, 28, 31, 30,31,30,31,31,30,31,30,31}; //13 size for simplicity
int months_l[13] = {0, 31, 29, 31, 30,31,30,31,31,30,31,30,31}; //leap year..
char *months_n[13]= {"", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

int cur_day(int m, int y){ // gives number corresponding to the day on 01/m/y :: 00 is Sunday.
  int is_leap = ((y%4 == 0 && y%100!=0) || y%400 == 0) ? 1 : 0;
  int days = 0; //no of days or days%7 passed since 1/1/1900
  /*int leap_years_passed = (y-1900)/4.0 + (y-1900)/400.0 - (y-1900)/100.0; */
  //above formula not working.
  int leap_years_passed = 0;
  for(int i = 1900; i< y; i++){
    if(((i%4 == 0 && i%100!=0) || i%400 == 0)) leap_years_passed++;
  }
  days+= y-1900; //365%7 = 1
  days+= (leap_years_passed%7);
  //reached 01/01/year
  for(int i = 1; i<m; i++){
    days += months[i];
  }//for : To add days passed till previous month.

  if(m>2 && is_leap) days+= 1;
  days%=7;
  days+=1; //to MAKE SUNDAY == 0

  days%=7;
  return days;
}
//
int main(void){//JAN 1 1900: Monday
  system("clear");
  printf("\t  CALENDAR\nMade By SUPREET SINGH -- CS16BTECH11038\n\n");
  menu();
  return 0;
}//main

void menu(void){
  //printing...
  printf("\nMENU: \n");
  printf("1: Display year\n2: Display month\n3: Move right\n4: Move left\n5: Exit\n");

  int ch;
  static int FLAG = 0; //for right/left functions.
  scanf("%d", &ch);
  if(ch<1 || ch>5){
    printf("Error! Enter your choice in the range 1-5\n");
    menu();
  }//validation

  switch(ch){ //process the input.
    case 1: //d_year
    printf("Enter a year: ");
    scanf("%d", &y);
    if(y<1900 || y>2016){
      printf("Error: Enter a year in the range 1900-2016\n");
      menu();
    } //year validation
    FLAG = 1; //FLAG for left/right functions
    d_year(y);
    menu();
    break;

    case 2: //d_month
    printf("Enter the month: ");
    //  int m = 0;
    scanf("%d", &m);
    if(m<1 || m> 12){
      printf("Error: Enter a valid month!\n");
      menu();
    }//month validation
    printf("Enter the year: ");
    //  int y = 0;
    scanf("%d", &y);
    if(y<1900 || y>2016){
      printf("Error: Enter a year in the range 1900-2016\n");
      menu();
    } //year validation
    FLAG =2;
    d_month(m, y);
    menu();
    break;

    case 3:
    right(FLAG);
    menu();
    break;

    case 4:
    left(FLAG);
    menu();
    break;

    case 5:
    printf("Thanks for using this calendar.\nBye!\n");
    exit(0);
  }//switch
}//menu

void d_month(int m, int y){ //one month has 28 char width...

  printf("\n    %s - %d\n", months_n[m], y);
  printf("Su  Mo  Tu  We  Th  Fr  Sa\n\n");
  int is_leap = ((y%4 == 0 && y%100!=0) || y%400 == 0) ? 1 : 0; //whether current year is leap.
  int col, x=cur_day(m,y); //col:==> DAY
  int p=0; //p is like a pointer ...
  for(col = 1; col<=(!is_leap ?months[m] : months_l[m]); col++){

    if(col==1){ //first day...
      p+=x;
      for(int i=0; i<x; i++) printf("    "); //for formatting purposes.
    }
    printf("%2d  ", col); //min 2 width
    p++;
    if(p%7==0) printf("\n\n");

  }//for
  putchar('\n');

}//d_month

void d_year(int y){
  int mc; //month-counter
  int is_leap = ((y%4 == 0 && y%100!=0) || y%400 == 0) ? 1 : 0; //if given year is leap.
  printf("%45d\n", y); //print the year...
  for(mc=1; mc<=10; mc+=3){
    int d1=1, d2=1, d3=1; //days of the months...
    int x1=cur_day(mc, y), x2=cur_day(1+mc, y), x3=cur_day(2+mc, y);
    printf("        %-24s", months_n[mc]); // - for left indentation
    printf("        %-24s", months_n[1+mc]);
    printf("        %-24s", months_n[2+mc]);
    putchar('\n');
    printf("Su  Mo  Tu  We  Th  Fr  Sa  ");
    printf("    |  Su  Mo  Tu  We  Th  Fr  Sa  |    ");
    printf("Su  Mo  Tu  We  Th  Fr  Sa  ");
    printf("\n\n");
    while(d3<=(!is_leap ?months[mc+2] : months_l[mc+2]) || d2<=(!is_leap ?months[mc+1] : months_l[mc+1]) || d1<=(!is_leap ?months[mc] : months_l[mc]))
    {//this loop runs as long as there are days to be printed..
      for(int p=0;p<21 ; p++){ //p is kind of a pointer, where to print the days.

        while(p<7 && d1<=(!is_leap ?months[mc] : months_l[mc])){//as long as days are left in the first month.
          if(d1==1){ //print the first day at correct pos.
            p+=x1;
            for(int i=0; i<x1; i++) printf("    ");

          }
          printf("%2d  ", d1); //min 2 width
          d1++;
          p++;
        }
        while(p<7){printf("    "); p++;} //if days are over, print spaces.

        if(d1==(!is_leap ?months[mc] : months_l[mc])+1 || d2<=(!is_leap ?months[mc+1] : months_l[mc+1]) || d3<=(!is_leap ?months[mc+2] : months_l[mc+2]))
        {
          printf("    |  ");
          if(d1==(!is_leap ?months[mc] : months_l[mc])+1) d1++; //last day , last row formatting.
        } //for formatting purpose.
        //first month formatted.

        while(p<14 && p>=7 && d2<=(!is_leap ?months[mc+1] : months_l[mc+1])){
          if(d2==1){
            p+=x2;
            for(int i=0; i<x2; i++) printf("    ");
          }
          printf("%2d  ", d2); //min 2 width
          d2++;
          p++;
        }
        while(p<14){printf("    "); p++;}

        if(d3<=(!is_leap ?months[mc+2] : months_l[mc+2]) || d2==(!is_leap ?months[mc+1] : months_l[mc+1]) +1 || d1==(!is_leap ?months[mc] : months_l[mc])+2)
          {
            printf("|    ");
            if(d2==(!is_leap ?months[mc+1] : months_l[mc+1])+1) d2++; //++ to print only once in case of last day.
            if(d1==(!is_leap ?months[mc] : months_l[mc])+2) d1++; //+2 in case last row only has the last day of leftmost month.
          }
        //second month formatted

        while(p<21 && p>=14 && d3<=(!is_leap ?months[mc+2] : months_l[mc+2])){
          if(d3==1){
            p+=x3;
            for(int i=0; i<x3; i++) printf("    ");
          }
          printf("%2d  ", d3); //min 2 width
          d3++;
          p++;
        }
        while(p<21){printf("    "); p++;}
      } //p for
      printf("\n\n");

    }//while dayss..
    if(mc<10) printf("%45d\n",y);
  } //mc for
} //d_year

void right(int f){
  if(f==0){
    printf("Error: You need to use either option 1/2 first.\n");
    return;
  }//1 or 2 not entered first.
  else if(f==1){ //year
    if(y==2016){
      printf("Sorry! Can only display calendar for the years 1900-2016. \n");
      return;
    }
    y+=1;
    d_year(y);
    return;
  }//f == 1
  else if(f==2){
    if(m==12 && y==2016){
      printf("Sorry! Can only display calendar for the years 1900-2016. \n");
      return;
    }
    if(m<12){
      d_month(m+1, y);
      m+=1;
      return;
    }
    else if(m==12){
      d_month(1, y+1);
      m=1;
      y+=1;
      return;
    }
  }//f==2
}//right()

void left(int f){
  if(f==0){
    printf("Error: You need to use either option 1/2 first.\n");
    return;
  }//1 or 2 not entered first.

  else if(f==1){ //year
    if(y==1900){
      printf("Sorry! Can only display calendar for the years 1900-2016. \n");
      return;
    }
    y-=1;
    d_year(y);
    return;
  }//f == 1

  else if(f==2){//month
    if(m==1 && y==1900){
      printf("Sorry! Can only display calendar for the years 1900-2016. \n");
      return;
    }
    if(m>1){
      d_month(m-1, y);
      m--;
      return;
    }
    else if(m==1){
      d_month(12, y-1);
      m=12;
      y-=1;
      return;
    }
  } //f==2
}//left
