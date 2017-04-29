

#ifndef JPLASM_LIST_H
#define JPLASM_LIST_H

#include "JplasmLabel.h"
#include "JplasmList.h"
#include <stdlib.h>
#include "MyList.h"


int jplasm_list_ctor(list_t* list);

int jplasm_list_dtor(list_t* list);

void jplasm_list_push(list_t* list, const unsigned char* label, size_t addr);

jplasm_label_elem_t* jplasm_list_pop(list_t* list);


#endif // JPLASM_LIST_H