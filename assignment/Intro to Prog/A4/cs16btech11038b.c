#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//memset(), strcpy etc

#define isdigit(x) (((x) >= '0' && (x) <= '9') ? 1 : 0)
#define SIZE 1002


//1000 + 1 for null character, +1 to check max size and error handling
//ASSUMPTION : ANSWER < SIZE (CHECKED!)
void strrev(char c[]);
void truncate(char num1[]){
  for(int i=(strlen(num1))-1; i>0; i--){ //Truncate leading zeroes.(Note leave atleast one zero.) >> i>0 not >=0
    if(num1[i]=='0') num1[i] = 0;
    else break;
  }
}
/*
In these functions, num1, num2 have 0th position > Least Sig Number.
In numcmp, 0th pos > Most sig number.
Result of addition is Reveresed(0th pos > Least Sig) {Because we need it in mult.}
Result of rest ops are unreversed(0th pos > Most sig).
*/
char *add(char *num1, char *num2, char *result);
char *sub(char *num1, char *num2, char *result); // note num1> num2
char *mult(char *num1, char *num2, char *result);
char *divi(char *num1, char *num2, char *result);
int numcmp(char *num1, char *num2);
int main(void){
  system("clear"); //in stdlib
  printf("BIG INT CALCULATOR\nBuilt by Supreet Singh CS16BTECH11038\n");

  /*MENU */

  while(1){//Inf loop for menu.
    printf("MENU: \n1) Add\n2) Multiply\n3) Divide\n4) Subtract\n5) Exit\nYour Choice: ");
    int choice = 0;
    if(!scanf("%d", &choice)){ //check if input is scanned
      scanf("%*s"); //scan and DISCARD non-integer input, if any/
      printf("ERROR: Invalid Input\n");
      continue;
    }
    else if(choice<1 || choice > 5){
      printf("Error: Invalid input\n");
      continue;
    }
    if(choice ==5){ //exit here(do not wait for switch) as number inputs are before switch.
      printf("Thanks for using this big int calculator!\nBye!\n");
      exit(0);
    }

    char num1[SIZE]="", num2[SIZE]=""; //char arrays for storing big int.

    char retry_flag = 0; //To act as a flag, go back to menu
    printf("Enter the first number: ");
    scanf("%s", num1);

    if(strlen(num1)>=SIZE-1){
      printf("ERROR: You entered too big a number\n");
      scanf("%*s"); //Discard characters remaining, if any.
      continue;
    }
    for(int i = 0; num1[i]!='\0'; i++){ //Validation.
      if(!isdigit(num1[i])){
        printf("Please enter a valid input number!\n");
        retry_flag =1;
        break;
      }
    } //Validation end.
    if(retry_flag) continue; //!

    printf("Enter the second number: ");
    scanf("%s", num2);
    if(strlen(num2)==SIZE-1){
      printf("ERROR: You entered too big a number\n");
      scanf("%*s"); //Discard characters remaining, if any.
      continue;
    }

    for(int i = 0; num2[i]!='\0'; i++){ //Validation 2.
      if(!isdigit(num2[i])){
        printf("Please enter a valid input number!\n");
        retry_flag =1;
        break;
      }
    } //Validation 2 end.
    if(retry_flag) continue; //goto beginning of loop.

    strrev(num1); //to make first digit least significant
    strrev(num2);
    truncate(num1);//intial zeros
    truncate(num2);

    char result[SIZE] ="0";

    switch(choice){
      case 1:{
        if(add(num1, num2, result)==NULL) break; //check if ans is within size.
        strrev(result);
        printf("The answer is: %s\n", result);
        break;
      }//Add

      case 2:{
        int n1=strlen(num1);
        int n2=strlen(num2);
        char *a = NULL;
        if(n1<n2) a = mult(num2, num1, result);
        else a = mult(num1, num2, result);


        if(a!=NULL)printf("The answer is: %s\n", a);
        break;
      }//Multiply

      case 3:{
        if(num2[0] == '0' && num2[1] == '\0'){
          printf("ERROR: Can't divide by ZERO\n");
          break;
        } //check for zero.
        int n1=strlen(num1);
        int n2=strlen(num2);
        if(n1<n2) {printf("The answer is: 0\n");}
        else if(n1>n2) {divi(num1, num2, result); printf("The answer is: %s\n", result);}
        else if(n1==n2){
          for(int i =n1-1; i>=0; i--){
            if(num1[i] < num2[i]) {printf("The answer is: 0\n"); break;} //BREAK for
            else if(num1[i] > num2[i]) {divi(num1, num2, result);printf("The answer is: %s\n", result); break;}
            else if(num1[i]==num2[i] && i == 0) { printf("The answer is: 1\n");break;}
          }//for
        } //elif n1 == n2

        break;
      }//Divide

      case 4:{
        int n1=strlen(num1);
        int n2=strlen(num2);
        if(n1<n2) {sub(num2, num1, result); printf("The answer is: -%s\n", result);}
        else if(n1>n2) {sub(num1, num2, result); printf("The answer is: %s\n", result);}
        else if(n1==n2){
          for(int i =n1-1; i>=0; i--){
            if(num1[i] < num2[i]) {sub(num2, num1, result); printf("The answer is: -%s\n", result); break;} //BREAK for
            else if(num1[i] > num2[i]) {sub(num1, num2, result);printf("The answer is: %s\n", result); break;}
            else if(num1[i]==num2[i] && i == 0) { printf("The answer is: 0\n");break;}
          }//for
        } //elif n1 == n2

        break;
      }//Subtract

      case 5:{ //not used anyway, done above.
        break;
      } //exit
    }//choice switch
  } //menu while ENDS

  return 0;
} //main

char *add(char *num1, char *num2, char *result){ //note: the nums here are reversed once!
  memset(result, '\0', SIZE);
  char carry = 0;
  for(int i = 0; i<SIZE; i++){
    if(num1[i]!=0 && num2[i]!=0){
      result[i] = ((num1[i]+num2[i]+carry - '0' - '0')%10)+'0';
      carry = (num1[i]+num2[i]+carry -'0' - '0')/10;
//      printf("%d -- %c, %d, %d--%c, %d--%c\n", result[i], result[i], carry, num1[i], num1[i], num2[i], num2[i]);
//for debugging purpose
    }
    else if((num1[i]==0 || num2[i] == 0) && !(num1[i]==0 && num2[i]==0)){ //kind of xor
      result[i] = ((num1[i]+num2[i]+carry - '0')%10) + '0';
      carry = (num1[i]+num2[i]+carry -'0' )/10;
//      printf("%d -- %c, %d, %d--%c, %d--%c\n", result[i], result[i], carry, num1[i], num1[i], num2[i], num2[i]);
    }
    else if(num1[i] == 0 && num2[i]==0 && carry!=0){
      result[i] = (carry%10) + '0';
      carry /= 10;
//      printf("%d -- %c, %d, %d--%c, %d--%c\n", result[i], result[i], carry, num1[i], num1[i], num2[i], num2[i]);
    }
    else{
      break;
    }
    if(i==SIZE-2){
      printf("ERROR: The result exceeded the maximum possible size!\n");
      return NULL;
    }
  }//for
  truncate(result);
  //leading zeros.

//  strrev(result);
  return result;
} //add

char *sub(char *num1, char *num2, char *result){//Here num1 > num2, also nums are reversed once.
  memset(result, '\0', SIZE); //simply for error checking.
  char carry=0;
  for(int i=0; i<SIZE; i++){
    if(num2[i]!='\0'){
      if(carry + num1[i] >= num2[i]){
      result[i] =carry + num1[i] - num2[i] + '0';
      carry = 0;
      }
      else{

        result[i]= carry + num1[i] - num2[i] + 10 + '0';
        carry = -1;
      }
    } //Null checker IF
    else if(num2[i]=='\0' && num1[i] != '\0'){
      if(num1[i] > '0'){
        result[i] = carry + num1[i]; //no need of + '0' here
        carry = 0;
      }
      else if(carry==-1){ //implied && num1[i] < 0
        result[i] = 10 + carry + '0'; // == 9, since carry == -1
        carry = -1;
      }
      else if(carry==0){
        result[i] = num1[i]; // == 0
        carry =0;
      }
    }
    else if(num2[i]=='\0' && num1[i] == '\0') break;

  }//for
  truncate(result);//leading zeros for.
  strrev(result);
  return result;
}

char *mult(char *num1, char *num2, char *result){
//  char temp_nums[strlen(num2)][SIZE]; //strlen(num2) == number of the temps you need.
  memset(result, '\0', SIZE);

    char *temp_nums = NULL;

//  printf("%s <> %s\n", num1, num2); //debugging

  for(int j=0; j< strlen(num2); j++){
//    char temp[SIZE]="";
    char *temp = (char*) calloc(SIZE, sizeof(char));
    temp_nums = (char *)calloc(SIZE, sizeof(char));
    char carry = 0;
    for(int i=0, k=0; ; i++){ //k -> pointer to element in num1, i -> pointer in temp_nums

      if(i < j){
        temp_nums[i] = '0';

      }//set beginning elements to zero.
      else if(num1[k]!='\0'){
        temp_nums[i] = (((num1[k]-'0')*(num2[j]-'0') + carry) % 10) + '0';
        carry = ((num1[k]-'0')*(num2[j]-'0') + carry) / 10;
//        printf("carry %d, num1[k] %c, num2[j] %c, temp_nums[j][i] <%s> %c %d %d\n", carry, num1[k], num2[j], temp_nums[j],temp_nums[j][i], j, i);
        k++;
      }
      else if(num1[k]=='\0' && carry!=0){
        temp_nums[i] = (carry %10) + '0';
        carry /= 10;
      }
      else if(num1[k] == '\0' && carry==0){
        temp_nums[i] = '\0';
        break; //out of inner loop.
      }
    }// for 2

    if(add(temp_nums, result, temp) == NULL){ //without the use of temp it gives strange results.
//      printf("ERROR: The answer exceeded the maximum possible size!\n");
      return NULL;
    } //if
    strcpy(result, temp);
    free(temp_nums);
    free(temp);
  }// for 1: Temp array
  strrev(result);
  return result;

} //mult

char *divi(char *num1, char *num2, char *result){ // num1 > num2 , also input is: a[0] -> least sig
  memset(result, '\0', SIZE);
  strrev(num1); // since our algo demands this. > NOT REVERSED NOW.
  //num2 is REVVED!
//  char quo[SIZE] = "", temp_nums[SIZE]="", rem[SIZE]="";
  char *quo = (char *) calloc(SIZE, sizeof(char));
  char *temp_nums = (char *) calloc(SIZE, sizeof(char));
  char *rem = (char *) calloc(SIZE, sizeof(char));
  int num1_size = strlen(num1);
  int num1_pt =0;
  for(int i=0; ; i++){
    if(num1_pt >= num1_size){ //To check if the calculation is done.
      strcpy(result, quo);
      strrev(result);
      truncate(result);//leading zeros for.

      strrev(result);
      free(quo);
      free(temp_nums);
      free(rem);
      return result;
    }//if calc has ended.
    //else:
    strcpy(temp_nums, rem);
    strncat(temp_nums, num1+num1_pt, 1); //temp_nums is set now!
    num1_pt++;
//    printf("Temp nums: %s, Rem: %s, Quo: %s, num1_pt: %d, num2: %s\n", temp_nums, rem, quo, num1_pt, num2);

    strrev(temp_nums); //revved
    for(int i=(strlen(temp_nums))-1; i>0; i--){ //Truncate leading zeroes.(Note leave atleast one zero.)
      if(temp_nums[i]=='0') temp_nums[i] = 0; //reverse needs to be done in order to delete the elements of the array.
      else break;
    }//leading zeros for.
    strrev(temp_nums); //un revved

    strrev(num2); //num2 unrevved now (for comparison)
    int cmp = numcmp(temp_nums, num2); //Don't use strcmp()
    strrev(num2); //again revved.

    if(cmp < 0){
      quo[i] = '0';
      strcpy(rem, temp_nums);
//    printf("t_n < num2 . \n");
      continue;
    }
    else if(cmp == 0){
//      printf("Equal nos. \n");
      quo[i] = '1'; //no need to call sub function this time.
      memset(rem, '\0', SIZE);
      continue;
    }
    else if(cmp > 0){
//      printf("t_n > num2 . \n");
      char *mul_res = (char *) calloc(SIZE, sizeof(char));
      char j[2] = "1"; //functions need a null terminated array.
      strrev(temp_nums); //again reversed >> needs to be passed into mult fxn.
      for(; j[0]<='9'; j[0]++){
        mult(num2, j, mul_res);
        strrev(mul_res); //mul_res is reversed now.
        sub(temp_nums, mul_res, rem);
        strrev(num2); //num2 is un reversed now.
        if(numcmp(rem, num2) < 0){ //for comparison, the no should be un rev.
          quo[i] = j[0];
          strrev(num2); //reverse num2 again.
          break;
        }
        strrev(num2); //reverse num2 again.
      } // for
      free(mul_res);
      strrev(temp_nums); //Now un reversed.
    } //elif
  }// for

  free(quo);
  free(temp_nums);
  free(rem);
  return NULL;
}//divi

int numcmp(char * num1, char *num2){ // both are not revved.
  int i = strlen(num1);
  int j = strlen(num2);
  if(i<j) return -1;
  else if(j>i) return 1;
  else if(i==j){
    for(int k=0; k<i; k++){
      if(num1[k] > num2[k]) return 1;
      else if(num1[k] < num2[k]) return -1;
      else if(num1[k] == num2[k] && i == k-1) return 0;
    }
  }
} //numcmp

void strrev(char c[]){ //strlen() doesnt include null character.
  char temp[strlen(c)+1];
  for(int i=0; i<strlen(c); i++) {
    temp[i] = c[strlen(c) - i -1];
  }
  temp[strlen(c)] = '\0';
  for(int i=0; i<strlen(c); i++) c[i] = temp[i];
  //c=temp; //wrong
}
