
#include "JplasmHashTable.h"
#include "MyHashTable.h"
#include "MyList.h"
#include <string.h>


size_t jplasm_hash_function(void* );
int jplasm_hash_table_elem_dtor(void* );
int jplasm_compare(void* , void* );

void __my_strncpy(unsigned char* dest, const unsigned char* from, size_t number); 



int jplasm_hash_table_ctor(hash_table_t* hash_table)
  {
    return hash_table_ctor(hash_table, HASH_TABLE_SIZE, jplasm_hash_function);
  }

int jplasm_hash_table_dtor(hash_table_t* hash_table)
  {
    return hash_table_dtor(hash_table, jplasm_hash_table_elem_dtor);
  }  
  
int jplasm_hash_table_push(hash_table_t* hash_table, const unsigned char* label, size_t addr)
  {
    jplasm_label_elem_t* new_elem = ( jplasm_label_elem_t* ) calloc(1, sizeof(*new_elem));
    __my_strncpy(new_elem -> label, label, MAX_LABEL_LENGTH);
    new_elem -> addr = addr;

    if ( hash_table_find(hash_table, new_elem, jplasm_compare) )
      return 1;
    else 
      hash_table_push(hash_table, new_elem);

    return 0;  
  }


jplasm_label_elem_t* jplasm_hash_table_find(hash_table_t* hash_table, jplasm_label_elem_t* elem)
  {
    list_node_t* found_elem = hash_table_find(hash_table, elem, jplasm_compare);

    if ( found_elem )
      return (jplasm_label_elem_t*)(found_elem -> elem);
    else 
      return NULL; 
  }    



size_t jplasm_hash_function(void* elem)
  {
    return (size_t)*(((jplasm_label_elem_t*)elem) -> label);
  }  

int jplasm_hash_table_elem_dtor(void* elem)
  {
    free(elem);

    return 0;
  }  


int jplasm_compare(void* elem1, void* elem2)
  {
    return strncmp(( (jplasm_label_elem_t*)elem1) -> label, ((jplasm_label_elem_t*)elem2) -> label, MAX_LABEL_LENGTH);
  }


void __my_strncpy(unsigned char* dest, const unsigned char* from, size_t number)
  {
    size_t cnt = 0;
    for ( cnt = 0; cnt < number && *(unsigned char*)(from + cnt) != ':'; cnt++)
      *(unsigned char*)(dest + cnt) = *(unsigned char*)(from + cnt);

    *(unsigned char*)(dest + cnt) = '\0'; 
  }  