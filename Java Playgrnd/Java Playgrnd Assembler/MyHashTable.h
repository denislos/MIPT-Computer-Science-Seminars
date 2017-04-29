
#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdlib.h>
#include "MyList.h"

typedef struct hash_table_t
  {
    size_t size;
    list_t* array;
    size_t (*hash_function)(void*);
  } hash_table_t;


int hash_table_ctor(hash_table_t* hash_table, size_t size, size_t (*hash_function)(void*) );

int hash_table_dtor(hash_table_t* hash_table, int (*hash_table_elem_dtor)(void* ));

void hash_table_push(hash_table_t* hash_table, void* elem);

list_node_t* hash_table_find(hash_table_t* hash_table, void* elem, int (*compare)(void* , void* ));


#endif //HASH_TABLE_H