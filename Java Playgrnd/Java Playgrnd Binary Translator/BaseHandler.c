
#include "BaseHandler.h"
#include "FileUtils.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>



enum BASE_ERROR
  {
    #define DEFINE_ERROR(error, num, msg) \
      error = num,
    
    #include "BaseHandler Errors.h"
      DUMMY_ERROR = -1
    
    #undef DEFINE_ERROR
  };





int base_onError(int error_code);

int loadToBase(base_t* base, const unsigned char* input_buffer, const size_t BUFFER_LENGTH);











int base_ctor(base_t* base, size_t size)
  {
    base -> array = ( base_item_t* ) calloc(size, sizeof(base_item_t));

    if ( !base -> array )
      base_onError(MEMORY_ALLOCATION_ERROR);

    base -> size = size;  

    return 0;   
  }






int base_load(base_t* base, const unsigned char* INPUT_FILE_NAME)
  {
    if (!INPUT_FILE_NAME)
      base_onError(INAPPROPRIATE_INPUT_FILE_NAME_ERROR);

    FILE* input_file = fopen(INPUT_FILE_NAME, "rb");
    if (!input_file)
      base_onError(CANNOT_OPEN_INPUT_FILE_ERROR);
    
    size_t input_buffer_length = 0;
    unsigned char* input_buffer = getInputBuffer(input_file, &input_buffer_length);
    if (!input_buffer)
      base_onError(MEMORY_ALLOCATION_ERROR);
    
    loadToBase(base, input_buffer, input_buffer_length);

    free(input_buffer);

    fclose(input_file);

    return 0;
  }


size_t base_insertInBuffer(base_t* base, unsigned char* buffer, size_t command, size_t position)
  {
    memcpy(buffer, base -> array[command].array[position].instrSet, base -> array[command].array[position].size);

    return base -> array[command].array[position].size;
  }





int base_dtor(base_t* base)
  {
    size_t cnt = 0;
    size_t cnt2 = 0;
    size_t base_size = base -> size;
    size_t instr_quantity = 0;

    for ( cnt = 0; cnt < base_size; cnt++)
      {
        instr_quantity =  base -> array[cnt].quantity;

        for ( cnt2 = 0; cnt2 < instr_quantity; cnt2++)
          free(base -> array[cnt].array[cnt2].instrSet);

        free(base -> array[cnt].array);  
      }

    free(base -> array);  

    return 0;
  }




int loadToBase(base_t* base, const unsigned char* input_buffer, const size_t BUFFER_LENGTH)
  {
    assert(base);
    assert(input_buffer);
    
    size_t input_pos = 0;
    size_t command = 0;

    while ( input_pos < BUFFER_LENGTH )
      {
        command = *(unsigned char*)(input_buffer + input_pos++);
         

        size_t tmp = base -> array[command].quantity = *(unsigned char*)(input_buffer + input_pos++);
        base -> array[command].array = (base_instr_t*)calloc(tmp, sizeof(base_instr_t));  

        size_t cnt = 0;
        for ( cnt = 0; cnt < tmp; cnt++)
          {

            size_t instr_size = base -> array[command].array[cnt].size = *(unsigned char*)(input_buffer + input_pos++);
            base -> array[command].array[cnt].instrSet = (unsigned char*)calloc(instr_size, sizeof(unsigned char));

            memcpy(base -> array[command].array[cnt].instrSet, input_buffer + input_pos, instr_size);
            input_pos += instr_size;         
          }
      }

    return 0;  
  }  










int base_onError(int error_code)
  {
    switch (error_code)
      {
        #define DEFINE_ERROR(error,num,msg) \
          case num:                         \
            fprintf(stderr, "%s\n", msg);   \
            exit(error_code);               \
            break;

        #include "BaseHandler Errors.h"  
          
          default:
            fprintf(stderr, "Unknown error\n");
            exit(1);
            break;

        #undef DEFINE_ERROR    
      }

    return error_code;
  }  