/*
Assignment 1 -- (b)
cs16btech11038
*/
#include <stdio.h>

int main(void){
    int marks, count = 0;
    printf("Please enter the marks: ");
    scanf("%d", &marks);
    printf("Your marks and the no. of conditional expressions executed are:\n");
    if(marks >= 0){
        count++;
        if (marks >= 31){
            count++;
            if (marks >= 41){
                count++;
                if(marks >= 51){
                    count++;
                    if(marks>=61){
                        count++;
                        if(marks >=71){
                            count++;
                            if(marks >= 76){
                                count++;
                                if(marks >= 81){
                                    count++;
                                   if(marks >= 91){
                                            count++; 
                                            if(marks >= 96){
                                                count++;
                                                if(marks >100){
                                                    count++;
                                                    printf("Please enter the correct marks\n");
                                                    return 0; 
                                                    /*
                                                    To end this program once this
                                                    output has been printed.
                                                    Otherwise it will continue
                                                    to print other grades
                                                    also.
                                                    */
                                                } // > 100 :: declare error
                                            printf("A+\n%d\n", count);   
                                            return 0; 
                                            } // >= 96 ends
                                            printf("A\n%d\n", count);    
                                            return 0; 
                                        }// >=91 ends
                                  printf("A-\n%d\n", count); 
                                    return 0; 
                                } // >=81
                                printf("B\n%d\n", count);  
                                return 0; 
                            } // >=76 ends
                            printf("B-\n%d\n", count);    
                            return 0; 
                        }// >=71
                        printf("D\n%d\n", count); 
                        return 0; 
                    }// >=61
                    printf("C-\n%d\n", count);  
                    return 0; 
                }// >= 51
                printf("D\n%d\n", count);    
                return 0; 
            }// >=41
            printf("FS\n%d\n", count);
            return 0; 
        } // >=31 end
        printf("FR\n%d\n", count);  
        return 0; 
    } // >=0 if
    else printf("Please enter the correct marks\n");
    
    return 0;
} // main ends