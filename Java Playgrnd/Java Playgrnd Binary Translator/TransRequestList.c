
#include "TransRequestList.h"
#include "MyList.h"
#include <stdlib.h>
#include <limits.h>



int trans_request_list_elem_dtor(void* );

int trans_request_list_compare_requested_pos(void* , void* );

int trans_request_list_compare_caller_pos(void* , void* );









void trans_request_list_ctor(list_t* list)
  {
    list_ctor(list);
  }


void trans_request_list_dtor(list_t* list)
  {
    list_dtor(list, trans_request_list_elem_dtor);
  }


void trans_request_list_add(list_t* list, size_t jpl_requested_pos, size_t jpl_caller_pos)
  {
    trans_request_list_elem_t* new_elem = (trans_request_list_elem_t*)calloc(1, sizeof(*new_elem));
    new_elem -> jpl_requested_pos = jpl_requested_pos;
    new_elem -> jpl_caller_pos = jpl_caller_pos;

    list_push_tail(list, new_elem);
  }  




void trans_request_list_add_elf_requested_pos(list_t* list, size_t jpl_requested_pos, size_t elf_requested_pos)
  {
    trans_request_list_elem_t list_elem;
    list_elem.jpl_requested_pos = jpl_requested_pos;

    list_node_t* list_node = list -> head;  
    while ( list_node )
      {
        if (trans_request_list_compare_requested_pos(list_node -> elem, &list_elem) == 0)
          ((trans_request_list_elem_t*)(list_node -> elem)) -> elf_requested_pos = elf_requested_pos;

        list_node = list_node -> next;
      }      
  }



void trans_request_list_add_elf_caller_pos(list_t* list, size_t jpl_caller_pos, size_t elf_caller_pos)
  {
    trans_request_list_elem_t* list_elem = (trans_request_list_elem_t*)calloc(1, sizeof(*list_elem));
    list_elem -> jpl_caller_pos = jpl_caller_pos;

    list_node_t* found_node = list_find(list, list_elem, trans_request_list_compare_caller_pos);
    ((trans_request_list_elem_t*)(found_node -> elem)) -> elf_caller_pos = elf_caller_pos;

    free(list_elem);
  }





trans_request_list_elem_t* trans_request_list_pop(list_t* list)
  {
    return (trans_request_list_elem_t*)list_pop_head(list);
  }
















int trans_request_list_elem_dtor(void* elem)
  {
    free(elem);
  }


int trans_request_list_compare_requested_pos(void* elem1, void* elem2 )
  {
    return (((trans_request_list_elem_t*)elem1) -> jpl_requested_pos - ((trans_request_list_elem_t*)elem2) -> jpl_requested_pos) % INT_MAX;
  }  


int trans_request_list_compare_caller_pos(void* elem1, void* elem2)
  {
    return (((trans_request_list_elem_t*)elem1) -> jpl_caller_pos - ((trans_request_list_elem_t*)elem2) -> jpl_caller_pos) % INT_MAX;
  }  
