

#include <assert.h>



#include "MyHashTable.h"



int hash_table_ctor(hash_table_t* hash_table, size_t size, size_t (*hash_function)(void* ))
  {
    assert(hash_table);
    
    hash_table -> size = size;
    hash_table -> array = ( list_t* ) calloc(size, sizeof(list_t));

    size_t cnt = 0;
    for ( cnt = 0; cnt < size; cnt++)
      list_ctor(hash_table -> array  + cnt );

    hash_table -> hash_function  = hash_function;

    return 0;   
  }



int hash_table_dtor(hash_table_t* hash_table, int (*hash_table_elem_dtor)(void* ))
  {
    assert(hash_table);

    size_t size = hash_table -> size;
    size_t cnt = 0;
    for ( cnt = 0; cnt < size; cnt++)
      list_dtor(hash_table -> array + cnt, hash_table_elem_dtor);

    hash_table -> size = 0;
    hash_table -> hash_function = NULL;
    
    return 0;
  }


void hash_table_push(hash_table_t* hash_table, void* elem)
  {
    assert(hash_table);

    list_push_head(hash_table -> array + hash_table -> hash_function(elem), elem);
  } 


list_node_t* hash_table_find(hash_table_t* hash_table, void* elem, int (*compare)(void* , void* ) )
  {
    assert(hash_table);
    
    return list_find(hash_table -> array + hash_table -> hash_function(elem), elem, compare);
  }     