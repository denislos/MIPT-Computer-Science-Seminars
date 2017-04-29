
#ifndef JPLASM_HASH_TABLE_H
#define JPLASM_HASH_TABLE_H

#include "JplasmLabel.h"
#include "MyHashTable.h"
#include <stdio.h>


#define HASH_TABLE_SIZE 2048



int jplasm_hash_table_ctor(hash_table_t* hash_table);

int jplasm_hash_table_dtor(hash_table_t* hash_table);

int jplasm_hash_table_push(hash_table_t* hash_table, const unsigned char* label, size_t addr);

jplasm_label_elem_t * jplasm_hash_table_find(hash_table_t* hash_table, jplasm_label_elem_t* elem);


#endif //JPLSAM_HASH_TABLE_H