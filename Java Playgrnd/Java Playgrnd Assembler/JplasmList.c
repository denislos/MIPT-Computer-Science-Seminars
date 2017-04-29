


#include "JplasmLabel.h"
#include "JplasmList.h"
#include <stdlib.h>
#include <string.h>
#include "MyList.h"

int jplasm_list_elem_dtor(void* );





int jplasm_list_ctor(list_t* list)
  {
    return list_ctor(list);
  }

int jplasm_list_dtor(list_t* list)
  {
    return list_dtor(list, jplasm_list_elem_dtor);
  }


void jplasm_list_push(list_t* list, const unsigned char* label, size_t addr)
  {
    jplasm_label_elem_t* new_elem = ( jplasm_label_elem_t* )calloc(1, sizeof(*new_elem));
    strncpy(new_elem -> label, label, MAX_LABEL_LENGTH);
    new_elem -> addr = addr;
    
    list_push_tail(list, new_elem);
  }  

jplasm_label_elem_t* jplasm_list_pop(list_t* list)
  {
    return (jplasm_label_elem_t*)list_pop_head(list);
  }  


int jplasm_list_elem_dtor(void* elem)
  {
    free(elem);
    return 0;
  }  

