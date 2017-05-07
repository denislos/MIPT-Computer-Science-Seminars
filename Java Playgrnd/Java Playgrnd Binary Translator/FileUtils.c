
#include "FileUtils.h"

#include <stdlib.h>
#include <stdio.h>


size_t filelength(FILE* file)
  {
    size_t file_begin = ftell(file);
    fseek(file, file_begin, SEEK_END);
    size_t file_length = ftell(file) - file_begin;
    fseek(file, file_begin, file_begin);
    return file_length;
  }




unsigned char* getInputBuffer(FILE* input_file, size_t* file_length)
  {
    *file_length = filelength(input_file);
    unsigned char* input_buffer = (unsigned char*)calloc(*file_length  + 1, sizeof(*input_buffer));
    if ( !input_buffer )
      return NULL; 
    
    fread(input_buffer, sizeof(unsigned char), *file_length, input_file);

    return input_buffer; 
  }