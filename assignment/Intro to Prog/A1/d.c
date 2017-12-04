/*
Assignment 1 -- (d)
cs16btech11038
*/
#include <stdio.h>

int main(void){
    int marks, count = 0;
    printf("Enter the marks: ");
    scanf("%d", &marks);
    printf("Your marks and the no. of conditional expressions executed are:\n");
    if(marks < 0) printf("Please enter the correct marks\n");
    else if(marks<=30) {
        count++;
        printf("FR\n%d\n", count);
    }
    else if(marks<=40) {
        count++;
        printf("FS\n%d\n", count);
    }
    else if(marks<=50) {
        count++;
        printf("D\n%d\n", count);
    }
    else if(marks<=60) {
        count++;
        printf("C-\n%d\n", count);
    }
    else if(marks<=70) {
        count++;
        printf("C\n%d\n", count);
    }
    else if(marks<=75) {
        count++;
        printf("B-\n%d\n", count);
    }
    else if(marks<=80) {
        count++;
        printf("B\n%d\n", count);
    }
    else if(marks<=90) {
        count++;
        printf("A-\n%d\n", count);
    }
    else if(marks<=95) {
        count++;
        printf("A\n%d\n", count);
    }
    else if(marks<=100) {
        count++;
        printf("A+\n%d\n", count);
    }
    else printf("Please enter the correct marks\n");
    
    return 0;
} // main ends