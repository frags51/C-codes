#include "heapsort.h"
#define INPUT_SIZE 4*1024 /* 4 KB : Size of input file*/

int main(void){
    FILE *fp=fopen(PATH, "rb");
    int _t, _rc=0;

    while(_rc<INPUT_SIZE/sizeof(uint32_t)){
      _rc++;
      fread(&_t, sizeof(uint32_t), 1, fp);
      printf("%d\n", _t);
    }
    fclose(fp);
}