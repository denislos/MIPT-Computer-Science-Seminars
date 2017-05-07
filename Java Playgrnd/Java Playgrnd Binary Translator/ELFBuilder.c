
#include "ELFBuilder.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "FileUtils.h"


enum BUILDER_REPLACE_SIMPLE_POINTER
  {
    SIMPLE_E_SHOFF = 0x28,
    SIMPLE_P_FILESZ = 0x60,
    SIMPLE_P_MEMSZ = 0x68,
    SIMPLE_SH_SIZE = 0x1e0,
    SIMPLE_SH_OFFSET_SHRSTRTAB = 0x218,
    SIMPLE_SH_OFFSET_SYMTAB = 0x258,
    SIMPLE_SH_OFFSET_STRTAB = 0x298
  };



enum BUILDER_REPLACE_IO_POINTER
  {
    IO_E_SHOFF = 0x28,
    IO_P_LOAD_TEXT_FILESZ = 0x60,
    IO_P_LOAD_TEXT_MEMSZ = 0x68,
    IO_P_LOAD_BSS_OFFSET = 0x80,
    IO_P_LOAD_BSS_VADDR = 0x88,
    IO_P_LOAD_BSS_PADDR = 0x90,
    IO_INPUT_BSS_ADDR = 0xb4,
    IO_OUTPUT_BSS_ADDR = 0x126,
    IO_SH_SIZE = 0x6c0,
    IO_SH_ADDR_BSS = 0x6f0,
    IO_SH_OFFSET_BSS = 0x6f8,
    IO_SH_OFFSET_SHRSTRTAB = 0x738,
    IO_SH_OFFSET_SYMTAB = 0x778,
    IO_SH_OFFSET_STRTAB = 0x7b8
  };



const unsigned char* ELF_SIMPLE_HEADING_PART_INPUT_FILE_NAME = "ELFHeadingPartSimple.bin";
const unsigned char* ELF_SIMPLE_ENDING_PART_INPUT_FILE_NAME = "ELFEndingPartSimple.bin";

const unsigned char* ELF_IO_HEADING_PART_INPUT_FILE_NAME = "ELFHeadingPartIO.bin";
const unsigned char* ELF_IO_ENDING_PART_INPUT_FILE_NAME = "ELFEndingPartIO.bin";




#define ELF_SIMPLE_HEADING_PART_LENGTH 0x80
#define ELF_SIMPLE_ENDING_PART_LENGTH 0x240

#define ELF_IO_HEADING_PART_LENGTH 0x19d
#define ELF_IO_ENDING_PART_LENGTH 0x643










unsigned char* getBuffer(size_t buffer_length);

void loadPart(unsigned char* output_buffer, const unsigned char* INPUT_FILE_NAME, size_t REQUIRED_LENGTH);

void notifyDataSIMPLEChanged(unsigned char* output_buffer, size_t BUFFER_LENGTH);

void notifyDataIOChanged(unsigned char* output_buffer, size_t BUFFER_LENGTH);












unsigned char* buildELF(const unsigned char* BUFFER, size_t buffer_length, size_t* elf_length, int isIONeeded)
  {
    unsigned char* output_buffer = NULL;

    #define BUILDER_CREATE_ELF(TYPE)                                                                                                                                                           \  
      {                                                                                                                                                                                        \
        output_buffer = getBuffer(ELF_##TYPE##_HEADING_PART_LENGTH + ELF_##TYPE##_ENDING_PART_LENGTH + buffer_length);                                                                         \
                                                                                                                                                                                               \                                                        
        loadPart(output_buffer, ELF_##TYPE##_HEADING_PART_INPUT_FILE_NAME, ELF_##TYPE##_HEADING_PART_LENGTH);                                                                                  \
                                                                                                                                                                                               \
        memcpy(output_buffer + ELF_##TYPE##_HEADING_PART_LENGTH, BUFFER, buffer_length);                                                                                                       \ 
                                                                                                                                                                                               \  
        loadPart(output_buffer + ELF_##TYPE##_HEADING_PART_LENGTH + buffer_length, ELF_##TYPE##_ENDING_PART_INPUT_FILE_NAME, ELF_##TYPE##_ENDING_PART_LENGTH);                                 \
                                                                                                                                                                                               \
        *elf_length = ELF_##TYPE##_HEADING_PART_LENGTH + buffer_length + ELF_##TYPE##_ENDING_PART_LENGTH;                                                                                      \
                                                                                                                                                                                               \
        notifyData##TYPE##Changed(output_buffer, buffer_length);                                                                                                                               \  
      }                                                    




    if ( isIONeeded )
      BUILDER_CREATE_ELF(IO)
    else 
      BUILDER_CREATE_ELF(SIMPLE)
 

    return output_buffer;

    #undef BUILDER_CREATE_ELF
  }






void loadPart(unsigned char* output_buffer, const unsigned char* INPUT_FILE_NAME, size_t REQUIRED_LENGTH)
  {
    FILE* input_file = fopen(INPUT_FILE_NAME, "rb");
    if ( !input_file )
      {
        fprintf(stderr, "Cannot open input file %s\n", INPUT_FILE_NAME);
        exit(1);
      }

    size_t file_length = filelength(input_file);
    if ( file_length != REQUIRED_LENGTH )
      {
        fprintf(stderr, "Bad input file\n");
        exit(1);
      } 

    fread(output_buffer, sizeof(unsigned char), REQUIRED_LENGTH, input_file);

    fclose(input_file);
  }




void notifyDataSIMPLEChanged(unsigned char* output_buffer, size_t BUFFER_LENGTH)
  {
    *(size_t*)(output_buffer + SIMPLE_E_SHOFF) += BUFFER_LENGTH;
    
    *(size_t*)(output_buffer + SIMPLE_P_FILESZ) += BUFFER_LENGTH;

    *(size_t*)(output_buffer + SIMPLE_P_MEMSZ) += BUFFER_LENGTH;

    *(size_t*)(output_buffer + SIMPLE_SH_SIZE + BUFFER_LENGTH) += BUFFER_LENGTH;

    *(size_t*)(output_buffer + SIMPLE_SH_OFFSET_SHRSTRTAB + BUFFER_LENGTH) += BUFFER_LENGTH;

    *(size_t*)(output_buffer + SIMPLE_SH_OFFSET_SYMTAB + BUFFER_LENGTH) += BUFFER_LENGTH;

    *(size_t*)(output_buffer + SIMPLE_SH_OFFSET_STRTAB + BUFFER_LENGTH) += BUFFER_LENGTH;
  }



void notifyDataIOChanged(unsigned char* output_buffer, size_t BUFFER_LENGTH)
  {
    *(size_t*)(output_buffer + IO_E_SHOFF) += BUFFER_LENGTH;
    *(size_t*)(output_buffer + IO_P_LOAD_TEXT_FILESZ) += BUFFER_LENGTH;
    *(size_t*)(output_buffer + IO_P_LOAD_TEXT_MEMSZ) += BUFFER_LENGTH;
    *(size_t*)(output_buffer + IO_P_LOAD_BSS_OFFSET) += BUFFER_LENGTH;
    *(size_t*)(output_buffer + IO_P_LOAD_BSS_VADDR) += BUFFER_LENGTH;
    *(size_t*)(output_buffer + IO_P_LOAD_BSS_PADDR) += BUFFER_LENGTH;
    *(size_t*)(output_buffer + IO_INPUT_BSS_ADDR) += BUFFER_LENGTH;
    *(size_t*)(output_buffer + IO_OUTPUT_BSS_ADDR) += BUFFER_LENGTH;
    *(size_t*)(output_buffer + IO_SH_SIZE + BUFFER_LENGTH) += BUFFER_LENGTH;
    *(size_t*)(output_buffer + IO_SH_ADDR_BSS + BUFFER_LENGTH) += BUFFER_LENGTH;
    *(size_t*)(output_buffer + IO_SH_OFFSET_BSS + BUFFER_LENGTH) += BUFFER_LENGTH;
    *(size_t*)(output_buffer + IO_SH_OFFSET_SHRSTRTAB + BUFFER_LENGTH) += BUFFER_LENGTH;
    *(size_t*)(output_buffer + IO_SH_OFFSET_SYMTAB + BUFFER_LENGTH) += BUFFER_LENGTH;
    *(size_t*)(output_buffer + IO_SH_OFFSET_STRTAB + BUFFER_LENGTH) += BUFFER_LENGTH;
  }




unsigned char* getBuffer(size_t buffer_length)
  {
    unsigned char* buffer = (unsigned char*)calloc(buffer_length, sizeof(*buffer));
    if (!buffer)
      {
        fprintf(stderr, "Memory allocation error\n");
        exit(1);
      }

    return buffer;  
  }