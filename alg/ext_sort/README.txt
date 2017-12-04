HEAPSORT ASSIGNMENT - README
============================

Compilation / execution instructions are for linux systems.

This program sorts in descending order, a large data set of unsigned 32-bit integers.
The integers are generated randomly in gen.c, which writes to "input.txt".
The final sorted output is written to "output.txt".

Compilation
-----------
- Generate the input from gen.c file. Compile it using: 
    $ gcc gen.c -o gen
    This generates a binary file called "input.txt", which contains the required number of integers.

- The sorting program can be compiled using:
    $ gcc heapsort.c

Execution
---------
- First generate the input:
    $ ./gen

    Note that since datasets are large, this input file may take some time to be generated.

- Run the program using (for EACH RUN please generate input with ./gen. To save time, a better 
    way would be maintain a back-up of input.txt, if same data set is needed to be sorted): 
    
    $ ./a.out 

    This creates another binary file called out.txt, which contains the sorted intgers.

Implementation Details
----------------------
This program works with binary files. Please note that they cannot be viewed in text editors as such, since
 they are not ASCII/unicode representations of the numbers.

gen.c generates random integers, and writes them to "input.txt"

The unsigned integer representation, in binary is written/read
 from the files. This program divides the input file size into CHUNKS number of chunks, and the size of 
 each chunk is CHUNK_SIZE (in bytes). The program sorts these individual chunks using heapsort, and
 overwrites these chunks to the same location.

The next part is merging these chunks together. This is done by maintaining an input buffer for each of the
 CHUNKS, and one output buffer. The output buffer occupies half of the MAX_MEM (maximum allowed memory) and 
 all the input buffers occupy the remaining half. The program compares the input buffers of each chunk, and
 write to the output buffer the value which is the maximum among all these. If the output buffer is full,
 it is written to the disc (out.txt). If the input buffers are full, they fetch the next part of the data
 from their corresponding chunks, until those chunks themselves run out of data. 
 
 fseek() is used to go to the appropriate locations to read/write.

In the end, all of the data is sorted.