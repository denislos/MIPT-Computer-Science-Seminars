
#include "Java Playgrnd Binary Translator.h"
#include "BaseHandler.h"
#include "ELFBuilder.h"
#include "TransRequestList.h"
#include "FileUtils.h"


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <limits.h>

 



enum TRANSLATOR_ERROR
  {
    #define DEFINE_ERROR(error, num, msg)  \
      error = num, 
    
    #include "Java Playgrnd Binary Translator Errors.h"
      DUMMY_ERROR = -1
    
    #undef DEFINE_ERROR
  };


const unsigned char* BASE_INPUT_FILE_NAME = "Base.bin";

const unsigned char TERMINATION_DIR = 0xFF;

#define DEFAULT_OUTPUT_BUFFER_SIZE 3500
#define COMMAND_QUANTITY 130







int onError(int error_code); 

unsigned char* setOutputBuffer(size_t size);

size_t getJPLHeader(const unsigned char* input_buffer, size_t input_buffer_length);

size_t convertToELF(unsigned char* input_buffer, unsigned char* output_buffer, int* isIONeeded);

void createTransitionRequests(list_t* list, const unsigned char* input_buffer);

void settleTransitionRequests(list_t* list, unsigned char* output_buffer);










#define isSimpleCommand(command) \
  ( command >= 0x11 && command <= 0x23 || command == 0x62 || command == 0x63 || command >= 0x70 && command <= 0x7b )


#define isMVWRTCommand(command) \
  ( command >= 0x24 && command <= 0x5f)


#define isTransitionCommand(command) \
  ( command >= 0x64 && command <= 0x6b )


#define isIOCommand(command) \      
  ( command == 0x60 || command == 0x61 )


#define isMOVNCommand(command) \
  ( command >= 0x2a && command <= 0x2f )


#define isPUSHNCommand(command) \
  ( command == 0x10)  


#define IO_OFFSET(command) \
  ( command == 0x60 ) ? IO_INPUT_OFFSET : IO_OUTPUT_OFFSET  








int translate(const unsigned char* INPUT_FILE_NAME, const unsigned char* OUTPUT_FILE_NAME)
  {
    if (!INPUT_FILE_NAME)
      onError(INAPPROPRIATE_INPUT_FILE_NAME_ERROR);
    if (!OUTPUT_FILE_NAME)
      onError(INAPPROPRIATE_OUTPUT_FILE_NAME_ERROR);  
    
    FILE* input_file = fopen(INPUT_FILE_NAME, "rb");
    if (!input_file)
      onError(CANNOT_OPEN_INPUT_FILE_ERROR);

    FILE* output_file = fopen(OUTPUT_FILE_NAME, "wb");
    if (!output_file)
      onError(CANNOT_OPEN_OUTPUT_FILE_ERROR);
    
    size_t input_buffer_length = 0;
    unsigned char* input_buffer = getInputBuffer(input_file, &input_buffer_length);
    if (!input_buffer)
      onError(MEMORY_ALLOCATION_ERROR);
    
    unsigned char* output_buffer = setOutputBuffer(DEFAULT_OUTPUT_BUFFER_SIZE + input_buffer_length);
    if (!output_buffer)
      onError(MEMORY_ALLOCATION_ERROR);


    size_t JPL_header_length = getJPLHeader(input_buffer, input_buffer_length);


    int isIONeeded = 0;
    size_t buffer_length = convertToELF(input_buffer + JPL_header_length, output_buffer, &isIONeeded);  



    size_t elf_file_length = 0;
    unsigned char* elf_file_buffer = buildELF(output_buffer, buffer_length, &elf_file_length, isIONeeded); 
    
    fwrite(elf_file_buffer, sizeof(unsigned char), elf_file_length, output_file);


    
    free(input_buffer);
    free(output_buffer);

    fclose(output_file);
    fclose(input_file);

    return 0;    
  }






size_t convertToELF(unsigned char* input_buffer, unsigned char* output_buffer, int* isIONeeded)
  {
    assert(input_buffer);
    assert(output_buffer);

    size_t input_pos = 0;
    size_t output_pos = 0;
    int64_t input_number = 0;
    unsigned char command = 0;

    const unsigned char REGISTER_AX = 0x11;
    const size_t PUSHN_INSERT_OFFSET_1 = 10;
    const size_t PUSHN_INSERT_OFFSET_2 = 4;

    base_t base;
    base_ctor(&base, COMMAND_QUANTITY);
    base_load(&base, BASE_INPUT_FILE_NAME);
    
    list_t list;
    trans_request_list_ctor(&list);

    createTransitionRequests(&list, input_buffer);
          
 
    while ( (command = *(unsigned char*)(input_buffer + input_pos)) != TERMINATION_DIR )
      {
        trans_request_list_add_elf_requested_pos(&list, input_pos++, output_pos);


        if (isSimpleCommand(command))
        
          output_pos += base_insertInBuffer(&base, output_buffer + output_pos, command, 0);
        
        else if (isMOVNCommand(command)) /* Should  be here because MVWRT encompasses it */
          {
        
            output_pos += base_insertInBuffer(&base, output_buffer + output_pos, command, 0);
            *(int64_t*)(output_buffer + output_pos) = *(int64_t*)(input_buffer + input_pos);

            input_pos += sizeof(int64_t);
            output_pos += sizeof(int64_t); 
        
          }
        else if (isMVWRTCommand(command))
          
          output_pos += base_insertInBuffer(&base, output_buffer + output_pos, command, *(unsigned char*)(input_buffer + input_pos++) - REGISTER_AX);
        
        else if (isTransitionCommand(command)) 
          {

            output_pos += base_insertInBuffer(&base, output_buffer + output_pos, command, 0); 

            trans_request_list_add_elf_caller_pos(&list, input_pos, output_pos);
            output_pos += sizeof(int);
            input_pos += sizeof(int64_t);
          
          }
        else if (isIOCommand(command))
          {

            *isIONeeded = 1;  

            output_pos += base_insertInBuffer(&base, output_buffer + output_pos, command, 0);            
            
            *(int*)(output_buffer + output_pos) = - ( ( IO_OFFSET(command) ) + output_pos + sizeof(int) + 1);

            output_pos += sizeof(int);
          
          }
        else 
          {

            input_number = *(int64_t*)(input_buffer + input_pos);
            input_pos += sizeof(int64_t);

            if ( input_number > INT_MAX )
              {
                output_pos += base_insertInBuffer(&base, output_buffer + output_pos,command,  1);
                *(size_t*)(output_buffer + output_pos - PUSHN_INSERT_OFFSET_1) = input_number;
              }
            else
              {   
                output_pos += base_insertInBuffer(&base, output_buffer + output_pos, command, 0);
                *(int*)(output_buffer + output_pos - PUSHN_INSERT_OFFSET_2) = input_number;
            
              }
          } 

      }

    settleTransitionRequests(&list, output_buffer);
    

    base_dtor(&base);
    trans_request_list_dtor(&list);
    
    return output_pos;
  } 









void createTransitionRequests(list_t* list, const unsigned char* input_buffer)
  {
    unsigned char command = 0;
    size_t input_pos = 0;

     while ( (command = *(unsigned char*)(input_buffer + input_pos++)) != TERMINATION_DIR )
       if (isSimpleCommand(command) || isIOCommand(command))
         ;
       else if (isMOVNCommand(command) || isPUSHNCommand(command))
         input_pos += sizeof(int64_t);
       else if (isMVWRTCommand(command))          
         input_pos++;
       else if (isTransitionCommand(command)) 
         {
           trans_request_list_add(list, *(size_t*)(input_buffer + input_pos), input_pos);
           input_pos += sizeof(int64_t); 
         }
       else 
         onError(UNKNOWN_COMMAND_ERROR);
  }




void settleTransitionRequests(list_t* list, unsigned char* output_buffer)
  {
    trans_request_list_elem_t* list_elem = NULL;

    while ( list_elem = trans_request_list_pop(list) )
      {
        *(int*)(output_buffer + list_elem -> elf_caller_pos) = (int)(list_elem -> elf_requested_pos - sizeof(int) - list_elem -> elf_caller_pos);

        free(list_elem);     
      }
  }














unsigned char* setOutputBuffer(size_t size)
  {
    unsigned char* output_buffer = (unsigned char*)calloc(size + 1, sizeof(*output_buffer));

    return output_buffer;
  }  





size_t getJPLHeader(const unsigned char* input_buffer, size_t input_buffer_length)
  {
    const size_t JPL_HEADER_LENGTH = 16;

    if ( input_buffer_length < JPL_HEADER_LENGTH )
      onError(BAD_JPL_FILE_ERROR);

    return JPL_HEADER_LENGTH;   
  }







int onError(int error_code)
  {
    switch (error_code)
      {
        #define DEFINE_ERROR(error, num, msg)     \
          case num:                               \
            fprintf(stderr, "%s\n", msg);         \
            exit(error_code);                     \
            break;
        
        #include "Java Playgrnd Binary Translator Errors.h"
          
          default:
            fprintf(stderr, "Unknown error\n");
            exit(1);
            break;
        
        #undef DEFINE_ERROR
      }

    return error_code;  
  }  