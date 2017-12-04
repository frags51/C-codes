CS1350 (OS - 1) Programming Assignment 1 - Readme
=================================================

Contains Compilation and Execution instructions (for linux systems).

The compiled program takes command-line argument (integer) as an input and forks a child process.
 The child process computes the Collatz Series for the number and writes it to a shared memory region.
 The main program then reads the shared memory and prints to stdout.   

Compilation
-----------
- Compile using the following command, with terminal open in same directory as the file:
    $ gcc Assgn1Src-cs16btech11038.c -lrt

- -lrt flag is used to tell the compiler to link librt (containing shared memory functions)

Execution
---------
-Execute the program with the command (N is a command line argument):
    $ ./a.out N
    
-Here N is a positive integer less than INT_MAX to compute collatz series for. 

-For example the following code:
    $ ./a.out 35
    gives the following output to stdout: 
        The Collatz Series for 35 is: 35 106 53 160 80 40 20 10 5 16 8 4 2 1

Error Handling
--------------
-The program has some error handling, so it will exit in case any of the shared memory / input operations
 fail, printing an appropriate error message to stderr.