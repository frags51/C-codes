#ifndef HEAPSORT
#define HEAPSORT
#define PATH "./input.txt"
#define OUT_PATH "./out.txt" /* Maybe open up a separate file for Output? */

#include<sys/types.h>
#include<stdio.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdint.h>
#include<string.h>

typedef struct heap
{
  uint32_t size;
  uint32_t buf[0];
}tHeap;

void reheap_insert(tHeap *max_heap, uint32_t temp);
uint32_t reheap_delete(tHeap *max_heap);

#endif
