/**
 * Output buffer is of size CHUNK_SIZE
 * Input buffers are of size CHUNK_SIZE/CHUNKS
 */ 
#include "heapsort.h"

#define MAX_MEM (1024L*1024*1024) /* 1 GB : 256*1024*1024 uint_32 */
#define INPUT_SIZE (4L*1024*1024*1024) /* 4 GB : Size of input file*/
#define CHUNKS (2L*(INPUT_SIZE)/(MAX_MEM)) /* == 8 Chunks in this case. Each will be 512 B */
#define CHUNK_SIZE ((INPUT_SIZE)/CHUNKS) /* == 512B in this case */
/* Rest 512 B to be used as output buffer, for merge */
#define OB_MLEN (CHUNK_SIZE/sizeof(uint32_t)) // Max length of the output buffer. Flush to out file after that.
#define IB_MLEN (CHUNK_SIZE/(CHUNKS*sizeof(uint32_t))) 

typedef struct{
  uint32_t buf[CHUNK_SIZE/(CHUNKS*sizeof(uint32_t))];
  int pos; // where we are reading the in buffer
  int bytes_done; // Number of bytes stored so far!
} i_buffer;

typedef struct{
  uint32_t buf[CHUNK_SIZE/sizeof(uint32_t)];
  int pos;
} o_buffer;

void fillIBuff(i_buffer *ib, int i, FILE *fp);
void merge(i_buffer *ibs[CHUNKS], o_buffer *ob, FILE *fp, FILE *op, int *rc);
void flush(o_buffer *ob, FILE *op);

/* ************* MAIN *********** */
int main(void) // Use int main(), not void --> C standard, should be followed.
{
  uint32_t temp = 0;
  tHeap *ptemp = NULL;
  tHeap *max_heap = NULL;
  long rc=0;

   /* Heap sort all the chunks */
    for(int c=0; c<(CHUNKS); c++){ // Read Chunks into the heap, and heap sort
      FILE *fp = fopen(PATH, "rb+");
  if (fp == NULL)
  {
    fprintf(stderr, "error in opening a file!\n");
    exit(1);
  }

      fprintf(stderr, "Iteration %d, CHunks: %d , CHunk size: %d ints\n", c, CHUNKS, CHUNK_SIZE/sizeof(uint32_t));
      if(fseek(fp, c*CHUNK_SIZE, SEEK_SET)!=0) fprintf(stderr, "Error in fseek\n");
      max_heap = (tHeap*) malloc(sizeof(tHeap));
      if(max_heap == NULL)
      {
        printf("error in Memory Allocation!\n");
        exit(1);
      }
      memset(max_heap, 0, sizeof(tHeap)); // init all to zero
      for(int j=0; j<CHUNK_SIZE/(sizeof(uint32_t)); j++){
        max_heap->size++;


        ptemp = realloc( max_heap, sizeof(tHeap) + (max_heap->size) * sizeof(uint32_t) );
        if(ptemp == NULL)
        {
          fprintf(stderr, "Re-allocation failed!\n");
          exit(1);
        }
        max_heap = ptemp;

        /*Insertion begins here:*/
        /*Step 1: Insert new element at the end.*/
        fread(&temp, sizeof(uint32_t), 1, fp);
        max_heap->buf[max_heap->size - 1] = temp;

        /*Step 2: To maintain the ordering property of heap, bubble up the largest element at its correct position*/
        reheap_insert(max_heap, temp);
        } //inner for
        fclose(fp);
        fp = fopen(PATH, "rb+");
      if(fseek(fp, c*CHUNK_SIZE, SEEK_SET)!=0) fprintf(stderr, "Error in fseek\n");
      while(max_heap->size > 0)
      {
        temp = reheap_delete(max_heap);
        rc++;
        fwrite(&temp, sizeof(uint32_t), 1, fp);
      }
  
      free(max_heap);
      fclose(fp);
    } // for loop
    
    fprintf(stderr, "Written in all: %ld ints\n", rc);
  
  /* Merge all chunks into an output buffer*/
    FILE *fp=fopen(PATH, "rb"); //Only need to read now!
    FILE *op=fopen(OUT_PATH, "wb");
    i_buffer *ibs[CHUNKS];
    for(int i=0; i<CHUNKS; i++) {ibs[i]=malloc(sizeof(i_buffer)); memset(ibs[i],0, sizeof(i_buffer));fillIBuff(ibs[i], i, fp);}
    o_buffer *ob = malloc(sizeof(o_buffer));
    memset(ob, 0, sizeof(o_buffer));

    int _trc = 0;
    while(_trc<(INPUT_SIZE/sizeof(uint32_t))){
      merge(ibs, ob, fp, op, &_trc);
    }

    free(ob);
    for(int i=0; i<CHUNKS; i++) {free(ibs[i]);}
    fclose(fp);
    fclose(op);

    // Testing
    /*
    op = fopen(OUT_PATH, "rb");
    int _t, _rc=0;
    while(_rc<INPUT_SIZE/sizeof(uint32_t)){
      _rc++;
      fread(&_t, sizeof(uint32_t), 1, op);
      printf("%d\n", _t);
    }
    fclose(op);
    printf("\n");
    _rc=0;
    fp=fopen(PATH, "rb");
    while(_rc<INPUT_SIZE/sizeof(uint32_t)){
      _rc++;
      fread(&_t, sizeof(uint32_t), 1, op);
      printf("%d\n", _t);
    }
    fclose(fp);
    */
  return 0;
}
void fillIBuff(i_buffer *ib, int i, FILE *fp){
  if(ib->bytes_done>=CHUNK_SIZE) /*memset(ib->buf, 0, sizeof(ib->buf)); */ return;
  else if(fseek(fp, i*CHUNK_SIZE+ (ib->bytes_done), SEEK_SET)!=0) fprintf(stderr, "Error in fseek\n");
  else{ // in case some elements left.
    ib->bytes_done=ib->bytes_done+(CHUNK_SIZE/CHUNKS);
    ib->pos=0; 
    for(int i=0; i<IB_MLEN; i++) fread(&ib->buf[i], sizeof(uint32_t), 1, fp);
  }
} //fillBuff

void merge(i_buffer *ibs[CHUNKS], o_buffer *ob, FILE *fp, FILE *op, int *rc){
  uint32_t max=0;
  int max_ind=-1;
  for(int i=0; i<CHUNKS; i++){
    
    if(ibs[i]->bytes_done >= CHUNK_SIZE && ibs[i]->pos==IB_MLEN) {fprintf(stderr,"\t>>buf %d used!\n", i);continue;} //don't check for this one now
    if(ibs[i]->pos==IB_MLEN){
      fillIBuff(ibs[i], i, fp);
    }
    if(ibs[i]->buf[ibs[i]->pos]>max){
      max=ibs[i]->buf[ibs[i]->pos];
      max_ind=i;
    } //if
  } //for
  if(max_ind!=-1) ibs[max_ind]->pos++;
  else fprintf(stderr,"\t\t>>>Error. Failed to find max.\n");
  if(ob->pos<OB_MLEN){
    ob->buf[ob->pos++]=max;
          if(max==0) fprintf(stderr, "Found a 0 max!\n");

  }
  else if(ob->pos==OB_MLEN){
    fprintf(stderr, "flushing obuffer, ob->pos== %d, ob->buf[pos-1]: %u!\n", ob->pos, ob->buf[ob->pos-1]);
    flush(ob, op);
    *rc = *rc + OB_MLEN;
  }
} //merge

void flush(o_buffer *ob, FILE *op){
  fwrite(ob->buf, sizeof(uint32_t), OB_MLEN, op);
  ob->pos=0;
}
void reheap_insert(tHeap *max_heap, uint32_t temp)
{
  uint32_t cur = 0; /*current index of heap element.*/
  uint32_t par = 0; /*parent index of current element.*/

  cur = max_heap->size - 1;

  while(cur > 0)
  {
    /*calculate parent index.*/
    par = (cur -1)/2;
    if(temp <= max_heap->buf[par])
    {
      /*element is already at correct position, so return from here.*/
      return;
    }
    else
    {
      /*since current element is greater than parent so swap.*/
      max_heap->buf[cur] = max_heap->buf[par];
      max_heap->buf[par] = temp;

      /*update current index*/
      cur = par;
    }
  }
}

uint32_t reheap_delete(tHeap *max_heap)
{
  uint32_t temp = 0; /*temporary variable*/
  uint32_t last = 0; /*last element in heap*/
  uint32_t cur = 0;  /*current index*/
  uint32_t left = 0; /*index of the left child of the current element*/
  uint32_t right = 0; /*index of the right child of the current element*/

  /*Step 1: store root in temporary variable.*/
  temp = max_heap->buf[0];
  last = max_heap->buf[max_heap->size - 1];

  /*decrement the size by 1*/
  max_heap->size = max_heap->size - 1;

  /*Step 2: reheap.*/
  /*initialize current, left and right indexes*/
  left = cur * 2 + 1;
  right = cur * 2 + 2;

  while(right < max_heap->size)
  {
    if(last >= max_heap->buf[left] && last >= max_heap->buf[right] )
    {
      return temp;
    }
    else if( (last < max_heap->buf[left]) && (max_heap->buf[left] > max_heap->buf[right]) )
    {
      max_heap->buf[cur] = max_heap->buf[left];
      cur = left;
    }
    else if( (last < max_heap->buf[right]) && (max_heap->buf[right] > max_heap->buf[left]) )
    {
      max_heap->buf[cur] = max_heap->buf[right];
      cur = right;
    }

    /*update indexes*/
    left = cur * 2 + 1;
    right = cur * 2 + 2;
  }

  if((left == max_heap->size - 1) && (last < max_heap->buf[left]))
  {
    max_heap->buf[cur] = max_heap->buf[left];
    cur = left;
  }
  max_heap->buf[cur] = last;
  return temp;
}

