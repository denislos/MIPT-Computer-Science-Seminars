

#ifndef TRANS_REQUEST_LIST_H
#define TRANS_REQUEST_LIST_H

#include "MyList.h"
#include <stdlib.h>

typedef struct 
  {
    size_t jpl_requested_pos;
    size_t elf_requested_pos;
    size_t jpl_caller_pos;
    size_t elf_caller_pos; 
  } trans_request_list_elem_t; 




void trans_request_list_ctor(list_t* list);

void trans_request_list_dtor(list_t* list);

void trans_request_list_add(list_t* list, size_t jpl_requested_pos, size_t jpl_caller_position);

void trans_request_list_add_elf_requested_pos(list_t* list, size_t jpl_requested_pos, size_t elf_requested_pos);

void trans_request_list_add_elf_caller_pos(list_t* list, size_t jpl_caller_pos, size_t elf_caller_pos);

trans_request_list_elem_t* trans_request_list_pop(list_t* list);



#endif //TRANS_REQUEST_LIST_H