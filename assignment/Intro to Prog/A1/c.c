/*
Assignment 1 -- (c)
cs16btech11038
*/
#include <stdio.h>


int main(void) {
    int marks, count = 0;
    printf("Please enter the marks: ");
    scanf("%d", &marks);
    printf("Your marks and the no. of conditional expressions executed are:\n");
    if(marks>=96&&marks<=100) {
        count++;
        printf("A+\n%d\n", count);
    }
    if(marks>=91&&marks<=95){
        count++;
        printf("A\n%d\n", count);
    }
    if(marks>=81&&marks<=90){
        count++;
        printf("A-\n%d\n", count);
    }
    if(marks>=76&&marks<=80){
        count++;
        printf("B\n%d\n", count);
    }
    if(marks>=71&&marks<=75){
        count++;
        printf("B-\n%d\n", count);
    }
    if(marks>=61&&marks<=70){
        count++;
        printf("C\n%d\n", count);
    }
    if(marks>=51&&marks<=60){
        count++;
        printf("C-\n%d\n", count);
    }
    if(marks>=41&&marks<=50){
        count++;
        printf("D\n%d\n", count);
    }
    if(marks>=31&&marks<=40){
        count++;
        printf("FS\n%d\n", count);
    }
    if(marks>=0&&marks<=30){
        count++;
        printf("FR\n%d\n", count);
    }
    else{
       count++;
       if(marks<0 || marks >100)
       printf("Please enter the correct marks\n");
    }
    return 0;
} // main ends
