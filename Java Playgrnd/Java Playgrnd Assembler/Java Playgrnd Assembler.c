
#include "Java Playgrnd Assembler.h"
#include "JplasmList.h"
#include "JplasmHashTable.h"
#include "JplasmLabel.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


enum ASSEMBLER_ERROR
  {
    #define DEFINE_ERROR(error, code, message) \
      error = code, 

    #include "Java Playgrnd Assembler Errors.h"
      DUMMY_ERROR  = -1
  
    #undef DEFINE_ERROR  
  };



enum ASSEMBLER_REGISTER
  {
    #define DEFINE_REGISTER(name, num) \
      name = num, 
    
    #include "Java Playgrnd Assembler Registers.h"
      
      DUMMY_REGISTER = -1

    #undef DEFINE_REGISTER   
  };


#define DEFAULT_OUTPUT_LENGTH 8192











unsigned char* getInputBuffer(FILE* input_file, size_t* input_file_length);

unsigned char* setOutputBuffer(size_t buffer_length);

int createJPLHeader(unsigned char* input_buffer);

int createBytecode(unsigned char* input_buffer, unsigned char* output_buffer, size_t* output_buffer_length);

size_t filelength(FILE* file);

int writeToFile(FILE* output_file, const unsigned char* output_buffer, size_t output_buffer_length); 

int findRegister(const unsigned char* register_name);

void addLabel(hash_table_t* hash_table, const unsigned char* label, size_t addr);

void addLabelRequest(list_t* list, const unsigned char* label, size_t addr);

void settleLabelRequests(unsigned char* output_buffer, hash_table_t* hash_table, list_t* list);

int onError(int error_code);







#define getBinaryCommand(name, offset)                                                                     \ 
 if ( 2 == sscanf(input_buffer + input_pos, "%s%s%n", input_string, input_extra_string, &input_length) )   \
  {                                                                                                        \
    input_pos += input_length;                                                                             \
                                                                                                           \
    if ( register_code = findRegister(input_string) )                                                      \
          *(unsigned char*)(output_buffer + output_pos - 1) = register_code + offset;                      \
    else                                                                                                   \
      onError(UNKNOWN_REGISTER);                                                                           \
                                                                                                           \ 
    if ( register_code = findRegister(input_extra_string) )                                                \
      *(unsigned char*)(output_buffer + output_pos++ ) = register_code;                                    \
    else                                                                                                   \ 
      onError(UNKNOWN_REGISTER);                                                                           \
                                                                                                           \
  }                                                                                                        \
else                                                                                                       \
  onError(name##_ERROR);                                                                                        




#define getLabelCommand(name)                                                                             \
  if ( 1 == sscanf(input_buffer + input_pos, "%s%n", input_string, &input_length) )                       \
    {                                                                                                     \
      input_pos += input_length;                                                                          \
                                                                                                          \
      addLabelRequest(&list, input_string, output_pos);                                                   \
                                                                                                          \
      output_pos += sizeof(int64_t);                                                                      \
    }                                                                                                     \
  else                                                                                                    \
    onError(name##_ERROR);                                             



#define getDataCommand(name, type)                                                                        \
  if ( 1 == sscanf(input_buffer + input_pos, "%x%n", &input_number, &input_length) )                      \
    {                                                                                                     \
      input_pos += input_length;                                                                          \
                                                                                                          \
      *(type*)(output_buffer + output_pos - 1) = input_number;                                            \
                                                                                                          \
      output_pos += sizeof(type) - 1;                                                                     \
    }                                                                                                     \
  else                                                                                                    \
    onError(name##_ERROR);







int assemble(const unsigned char* input_file_name, const unsigned char* output_file_name)
  {
    if (!input_file_name)
      return onError(INAPPROPRIATE_INPUT_FILE_NAME_ERROR);
    if (!output_file_name)
      return onError(INAPPROPRIATE_OUTPUT_FILE_NAME_ERROR);  

    FILE* input_file = fopen(input_file_name, "r");
    if (!input_file)
      return onError(CANNOT_OPEN_INPUT_FILE_ERROR);
    FILE* output_file = fopen(output_file_name, "wb");
    if (!output_file)
      return onError(CANNOT_OPEN_OUTPUT_FILE_ERROR);

    size_t input_buffer_length = 0;
    size_t output_buffer_length = 0;
    unsigned char* input_buffer = getInputBuffer(input_file, &input_buffer_length);
    if ( !input_buffer )
      return onError(MEMORY_ALLOCATION_ERROR);

    unsigned char* output_buffer = setOutputBuffer(DEFAULT_OUTPUT_LENGTH +  3 * input_buffer_length);
    if ( !output_buffer )
      return onError(MEMORY_ALLOCATION_ERROR);
    
    size_t header_length = createJPLHeader(output_buffer);
    int errno = createBytecode(input_buffer, output_buffer + header_length, &output_buffer_length);

    writeToFile(output_file, output_buffer, output_buffer_length + header_length);  

    
    free(input_buffer);
    free(output_buffer);

    fclose(input_file);
    fclose(output_file);

    return 0;
  }




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
    assert(input_file);
    assert(file_length);

    *file_length = filelength(input_file);

    unsigned char* input_buffer = (unsigned char*)calloc(*file_length  + 1, sizeof(*input_buffer));
    if ( !input_buffer )
      return NULL; 
    
    fread(input_buffer, sizeof(unsigned char), *file_length, input_file);

    return input_buffer; 
  }





unsigned char* setOutputBuffer(size_t buffer_length)
  {
    unsigned char* output_buffer = (unsigned char*)calloc(buffer_length, sizeof(*output_buffer));

    return output_buffer;
  }




int createJPLHeader(unsigned char* output_buffer)
  {
    assert(output_buffer);
    
    const size_t header_length = 16;

    unsigned char* header = (unsigned char*)calloc(header_length, sizeof(*header));
    strcpy(header, ".JPL");

    strncpy(output_buffer, header, header_length);  

    return header_length;
  }



int createBytecode(unsigned char* input_buffer, unsigned char* output_buffer, size_t* output_buffer_length)
  {
    assert(input_buffer);
    assert(output_buffer);
    assert(output_buffer_length);

    size_t input_pos = 0;
    size_t output_pos = 0;
    
    int64_t input_number = 0;
    size_t input_length = 0;
    unsigned char input_string[MAX_LABEL_LENGTH] = "";
    unsigned char input_extra_string[MAX_LABEL_LENGTH] = "";
    size_t input_command_length  = 0; 
    int register_code  = 0;

    hash_table_t hash_table;
    jplasm_hash_table_ctor(&hash_table);

    list_t list;
    jplasm_list_ctor(&list);


    while ( 1 == sscanf(input_buffer + input_pos, "%1023s%n", input_string, &input_command_length) )
      {
        input_pos += input_command_length;
        

        #define DEFINE_COMMAND(name, number, code)                      \
          if ( strcmp(input_string, #name) == 0 )                       \   
            {                                                           \                                                          
              *(unsigned char*)(output_buffer + output_pos++) = number; \   
              code                                                      \   
            }                                                           \   
          else  

        #include "Java Playgrnd Assembler Commands.h"
          
          if ( *(unsigned char*)(input_buffer + input_pos - 1) == ':' )
            addLabel(&hash_table, input_string, output_pos);
          else
            onError(UNKNOWN_COMMAND_ERROR);
       
      } 

    settleLabelRequests(output_buffer, &hash_table, &list);  

    *(unsigned char*)(output_buffer + output_pos++) = 0xFF;
    
    *output_buffer_length = output_pos;

    jplasm_hash_table_dtor(&hash_table);
    jplasm_list_dtor(&list);
  }



int writeToFile(FILE* output_file, const unsigned char* output_buffer, size_t output_buffer_length)
  {
    assert(output_file);
    assert(output_buffer);

    fwrite(output_buffer, sizeof(*output_buffer), output_buffer_length, output_file);

    return 0;
  }


int findRegister(const unsigned char* register_name)
  {
    assert(register_name);
    
    int register_code = 0;

    #define DEFINE_REGISTER(name, num) \
      if ( strcmp(#name, register_name) == 0 )   \
        register_code = num;                     \
      else 
     #include "Java Playgrnd Assembler Registers.h"
       ;

     #undef DEFINE_REGISTER;

    return register_code;    
  }


void addLabel(hash_table_t* hash_table, const unsigned char* label, size_t addr)
  {
    if (jplasm_hash_table_push(hash_table, label, addr) )
      onError(MULTIPLE_LABEL_ERROR);
  }


void addLabelRequest(list_t* list, const unsigned char* label, size_t addr)
  {
    jplasm_list_push(list, label, addr);
  }  

void settleLabelRequests(unsigned char* output_buffer, hash_table_t* hash_table, list_t* list)
  {
    assert(output_buffer);
    assert(hash_table);
    assert(list);

    jplasm_label_elem_t* found_elem = NULL;
    jplasm_label_elem_t* label_request = NULL;

    while ( label_request = jplasm_list_pop(list) )
      if ( found_elem = jplasm_hash_table_find(hash_table, label_request) )
        {
          *(int64_t*)(output_buffer + label_request -> addr ) = found_elem -> addr;
          free(label_request);
        }
      else 
        onError(LABEL_NOT_FOUND_ERROR);
  }



int onError(int error_code)
  {
    switch ( error_code )
      {
        #define DEFINE_ERROR(error_name, code, message) \
          case code:                                    \
            fprintf(stderr, "%s\n", message);             \
            exit(code);                                   \
            break;

        #include "Java Playgrnd Assembler Errors.h"
          default:
            fprintf(stderr, "UNKNOWN ERRROR");
            break; 

        #undef DEFINE_ERROR       
      }

    return error_code;  
  }
