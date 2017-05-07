
#ifndef ELF_BUILDER_H
#define ELF_BUILDER_H

#include <stdlib.h>

enum BUILDER_IO_ADDR /* It is the offset from the end of heading part */
  {
    IO_INPUT_OFFSET = 0xec,
    IO_OUTPUT_OFFSET = 0x78
  };


unsigned char* buildELF(const unsigned char* BUFFER, size_t buffer_length, size_t* elf_length, int isIONeeded);

#endif //ELF_BUILDER_H