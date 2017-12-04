/*
CS16BTECH11038 :: Expression evaluator.
Given: 1 <= Input <= 10^9 !! (ALSO Checked for error.)
*/
#include <stdio.h>
#include <stdlib.h>
//for exit, atexit system, calloc realloc etc. atol also used.
#define is_digit(x) (((x) >= '0' && (x) <= '9') ? 1 : 0)
/*
These global variables are accessed by the 'stack' functions.
*/
long int *nums = NULL;
int num_pt = 0; //next free pos in nums.
char *ops = NULL;
int ops_pt = 0; //next free pos in ops.
int size_n = 0;
int size_op = 0;

void push_n(long int a);
long int pop_n(void);
void push_op(char c);
char pop_op(void);
int scan_next(char s[]);
int eval(void);

void free_mem(void){ //execed at the exit of the program.
  free(nums);
  free(ops);
}
int main(void){
  atexit(free_mem); //in stdlib
  system("clear");
  nums = (long int *) calloc(32, sizeof(long int));
  ops = (char *) calloc(32, sizeof(char));
  size_n+= 32;
  size_op += 32;
  printf("EXPRESSION EVALUATOR\nMade by Supreet Singh  -- CS16BTECH11038\n\n");
  printf("Enter your input numbers in range: 1 <= num <= 10^9\n");
  printf("Enter your expression: ");

  eval();
  printf("Result is: %ld\n", pop_n());
  return 0;
} //main

int scan_next(char s[]){ // reads input character by character, if a number then puts it into the string s.
  char c;
  while((s[0] = c = getchar()) == ' ' || c =='\t'); //skip blank spaces, and load the last blank space/character after last blank space into s[0]
  s[1] = '\0';
//  printf("frist s[0] = %c\n", s[0]);
  if(!is_digit(c)) return c; //in case c is not a number, then return c.
  int i = 0;
  char t = 0; //zeroes romeval.
  if(is_digit(c)) while(is_digit(s[i])) {
//    printf("i : %d, s[i] : %c, t: %d\n", i, s[i], t); //FOR DEBUGGING
//    c = getchar();

    if(s[0]=='0' && t==0) {while((c=getchar()) == '0'); t= 1;}//check for excess zeroes.
    else c = getchar();
    i++;
    if(i<9 || (i==9 && c=='0' &&s[0]=='1' &&s[1]=='0'&&s[2]=='0'&&s[3]=='0'&&s[4]=='0' &&s[5]=='0'&&s[6]=='0'&&s[7]=='0'&&s[8]=='0')\
  || (i==10  && !is_digit(s[i]))) s[i] = c; //input checker.
    else {
      printf("ERROR: Too large a number!\n");
      exit(1);
    }
  } //get the number, plus an extra character.
  s[i] = '\0'; //earlier s[i] was that extra char.
  ungetc(c, stdin); //because an extra char was read.
  return 0; //(in case a number is read.)
}

void push_n(long int a){
  if(num_pt >= size_n -1) {
    size_n +=32;
    nums = (long int *) realloc(nums, size_n * sizeof(long int));
  }
//  printf("Pushed : %d\n", a);
  nums[num_pt++] = a;
}

long int pop_n(void){
  if(num_pt > 0) {/*printf("Popped: %ld\n", nums[num_pt-1]);*/return nums[--num_pt];}
  else {
    printf("ERROR: No number to pop\n");
    exit(0);
    return 0;
  }
}
void push_op(char c){
  if(ops_pt >= size_op -1) {
    size_op +=32;
    ops = (char *) realloc(ops, size_op * sizeof(char));
  }
//  printf("Pushed: %c\n", c);
  ops[ops_pt++]= c;
}
char pop_op(void){
  if(ops_pt > 0) {/*printf("Popped: %c\n", ops[ops_pt-1]);*/return ops[--ops_pt];}
  else {
    printf("ERROR: No operator to pop\n");
    exit(0);
    return 0;
  }
}//pop_op
int eval(void){//returns 0 on success, 1 if bracket was closed.
  static int call_count = 0; //to check brackets closed / open.
  static int pointer = 0;//To display what point error occured at.
//  printf("---------\n");
  int rb = 0; // 2: if next is * /, 1: next is + -//to check preedence operators
  int prev_in = -1; //0: number, 1:op+/-,2:op*/, 3:bo, 4: bc
  char c, s[12];
  while((c = scan_next(s)) != '\n'){//next input is not new line.
    //  int  k = 1;
//    printf("Now in while: (%d times) %c\n",k++, c);
    pointer++;
    switch(c){
      case '(':{ // OPENING BRACKET.
        if(prev_in == 0){
          printf("ERROR: Invalid expression (number before bracket)\n");
          printf("ERROR near number/operator no: %d\n", pointer);
          exit(1);
        }
        call_count++;
        if(eval() == 1) { //if bracket close detected.
          goto label;
        }
        else {
          printf("ERROR: Bracket(s) not closed!\n");
          printf("ERROR near number/operator no: %d\n", pointer);
          exit(1);
        }
        break;
      } //( ENDS

      case ')':{
        call_count--;
//        printf("--------\n"); //for debugging
        if(prev_in !=0 || call_count<0){//prevent additional ) signs.
          printf("ERROR: Expression is invalid. An extra bracket, perhaps. Or a bracket closed prematurely.\n");
          printf("ERROR near number/operator no: %d\n", pointer);
          exit(1);
        }
        return 1; //SUCCESSfully closed bracket.
        break;
      }// ) ends

      case 0: { //For a number
        if(prev_in == 0){//Number after number.
          printf("ERROR: Please enter a valid expression.\n");
          printf("ERROR near number/operator no: %d\n", pointer);
          exit(1);
        }//if.

        push_n(atol(s)); //put this number onto num stack.
label: //treat last pushed number as if it was input by the user.
          if(prev_in == 2){ // operator: * OR /
          while((c=getchar()) == ' ' || c =='\t'); //skip blank spaces
          if(rb==2 && (c == '+' || c == '-' || c=='\n' || c==')')) rb = 1; //rb==2 is neccesary.
          ungetc(c, stdin);
          switch(pop_op()){
            case '*':
            {
              push_n(pop_n() * pop_n());
              break;
            }
            case '/':{ //order of numbers matters here.
              int nx = pop_n(); //here order matters.
              if(nx == 0){
                printf("ERROR: Can't divide by ZERO\n");
                printf("ERROR near number/operator no: %d\n", pointer);
                exit(1);
              }
              push_n(pop_n()/nx);

              break;
            }
          }//* or / ?
        } //if prev was * or /
        if(prev_in == 1 || (rb == 1 && prev_in==2)){ //prev entered was + - or returned from mult.
        while((c=getchar()) == ' ' || c =='\t'); //white space skipper.
                      //needs to be checked since */ have higher priority.
          if(c == '*' || c == '/') { //this will always be false if rb==1.
            rb = 2;
            ungetc(c, stdin); //needs to be done, since we have to read the next character.
          }
          else {
            ungetc(c, stdin); //again, it needs to be done.
            switch(pop_op()){
              case '+':{
                push_n(pop_n()+pop_n());
                break;
              }
              case '-':{//again, order matters.
                int nt = pop_n();
                push_n(pop_n() - nt);
                break;
              }
            } //swithc for operator
          }
        }// elif + -
        prev_in = 0;
        break;
      } // case number ends

      case '+': case '-': //add ops into the array.
      {
        if(prev_in != 0){
          printf("ERROR: Expression/Number expected before + or -\n");
          printf("ERROR near number/operator no: %d\n", pointer);
          exit(1);
        }

        push_op(c);
        while((c=getchar()) == ' ' || c =='\t'); //check if next input is a number
        if(!is_digit(c) && c!= '('){
          printf("ERROR: Expression/Number expected after + or -\n");
          printf("ERROR near number/operator no: %d\n", pointer);
          exit(1);
        }
        ungetc(c, stdin);
        prev_in = 1;

        break;
      }// case +/-
      case '*': case '/':{ //simply add the operators into the array.
        if(prev_in !=0){
          printf("ERROR: Expression/Number expected before * or /\n");
          printf("ERROR near number/operator no: %d\n", pointer);
          exit(1);
        }
        push_op(c);
        while((c=getchar()) == ' ' || c =='\t');
        if(!is_digit(c) && c!= '('){ //check whether the next thing is a number
          printf("ERROR: Expression/Number expected after * or /\n");
          printf("ERROR near number/operator no: %d\n", pointer);
          exit(1);
        }
        ungetc(c, stdin);
        prev_in = 2;
        break;
      }//case *, /

//      case ' ': case '\t': break;
      default:{
        printf("ERROR: Enter a valid expression.\n");
        printf("ERROR near number/operator no: %d\n", pointer);
        exit(1);
        break;
      }
    } //switch
  } //while.

return 0; //SUCCESS.
} //eval
