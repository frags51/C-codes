CS1350 (OS - 1) Programming Assignment 2 - Readme
=================================================

Contains Compilation and Execution instructions (for linux systems).

Both Primes-proc and Primes-th take two integer arguments ('n' and 'k') as input from stdin (Help messages are printed
to tell the user when to input what). Primes-proc then creates 'k' child processes, and distributes (almost) equally numbers
from 1 to n-1 to each of the 'k' processes, each of which checks if those numbers are prime or not. The prime numbers
are outputted to a file, and it prints to stdout the time taken to perform this computation. Primes-th does the same thing,
but with threads.

Both programs are written in C++11 standard.

Compilation
-----------
- Compile the programs using the following commands, with terminal open in same directory as the files:

    -For Primes-proc:
    $ g++ Assgn2-Primes-proc-cs16btech11038.cpp -lrt -std=c++11

    -For Primes-th:
    $ g++ Assgn2-Primes-th-cs16btech11038.cpp -pthread -std=c++11

Execution
---------
Execute each of the program with the following commands:

    $ ./a.out

Both programs will then ask the user for an integer 'n': The number upto which all prime numbers should be calculated.
Then they ask the user for an integer 'k' (2<=k<=32): The numbers of child processes/threads they should create.

Both programs output to stdout the time taken in seconds to compute the prime numbers.
Primes-proc outputs all the prime numbers to the file Primes-proc.txt
Primes-th outputs all the prime numbers to the file Primes-th.txt

Error Handling
--------------
-The programs have some error handling, so it will exit in case any of the shared memory / input operations
 fail, printing an appropriate error message to stderr.
