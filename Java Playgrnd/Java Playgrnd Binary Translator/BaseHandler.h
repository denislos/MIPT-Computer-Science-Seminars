

#ifndef BASE_HANDLER_H
#define BASE_HANDLER_H

#include <stdlib.h>


typedef struct base_instr_t
  {
    size_t size;
    unsigned char* instrSet;
  } base_instr_t; 


typedef struct base_item_t
  {
    size_t quantity;
    base_instr_t* array;
  } base_item_t;

typedef struct base_t
  {
    size_t size;
    base_item_t* array;
  } base_t; 




int base_ctor(base_t* base, size_t size);

int base_load(base_t* base, const unsigned char* INPUT_FILE_NAME);

size_t base_insertInBuffer(base_t* base, unsigned char* buffer, size_t command, size_t position);

int base_dtor(base_t* base);







#endif //BASE_HANDLER_H