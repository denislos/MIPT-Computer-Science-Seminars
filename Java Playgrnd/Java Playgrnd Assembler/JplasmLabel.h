
#ifndef JPLASM_LABEL_H
#define JPLASM_LABEL_H

#include <stdio.h>

#define MAX_LABEL_LENGTH 1024

typedef struct jplasm_label_elem_t
  {
    unsigned char label[MAX_LABEL_LENGTH];
    size_t addr;
  } jplasm_label_elem_t;


#endif //JPLASM_LABEL_H