
#include "Java Playgrnd Virtual Machine.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>


enum VIRTUAL_MACHINE_ERROR
  {
    #define DEFINE_ERROR(name, code, msg) \
      name = code,

    #include "Java Playgrnd Virtual Machine Errors.h"
      DUMMY_ERROR = -1

    #undef DEFINE_ERROR    
  };




int loadToRAM(virtual_machine_t* virtual_machine, const unsigned char* input_file_name);

size_t filelength(FILE* file);

unsigned char* getInputBuffer(FILE* file, size_t* file_length);

size_t readJPLHeader(unsigned char* input_buffer, size_t input_buffer_length);

int64_t getFromRegister(virtual_machine_t* virtual_machine, unsigned char reg);

void copyToRAM(virtual_machine_t* virtual_machine, const unsigned char* input_buffer, size_t input_buffer_length);

int onError(int error_code);




#define getPUSHNCommand()                                      \
  RSP -= sizeof(int64_t);                                      \
  *(int64_t*)(VRAM + RSP) = *(int64_t*)(VRAM + RIP);           \
  RIP += sizeof(int64_t);

#define getPUSHRCommand(reg)                                   \
  *(int64_t*)(VRAM + RSP - sizeof(int64_t) ) =  R##reg;         \
  RSP -= sizeof(int64_t);   

#define getPOPCommand(reg)                                     \
  R##reg = *(int64_t*)(VRAM + RSP);                             \
  RSP += sizeof(int64_t);

#define getMOVRCommand(reg)                                    \  
  R##reg = getFromRegister(virtual_machine, VRAM[RIP++]);       

#define getMOVNCommand(reg)                                    \
  R##reg = *(int64_t*)(VRAM + RIP);                            \
  RIP += sizeof(int64_t); 

#define getMOVDataCommand(reg, type)                                                 \
  R##reg = (int64_t)*(type*)(VRAM + getFromRegister(virtual_machine, VRAM[RIP++]) );  

#define getWRTDataCommand(reg, type)                                                 \
  *(type*)(VRAM + R##reg) = getFromRegister(virtual_machine, VRAM[RIP++]);  

#define getOffsetCommand(CONDITION)                                                    \
  if ( *(int64_t*)(VRAM + RSP) CONDITION *(int64_t*)(VRAM + RSP + sizeof(int64_t) ) )  \
    RIP = *(int64_t*)(VRAM + RIP);                                                     \
  else                                                                                 \
    RIP += sizeof(int64_t);                                                            \
                                                                                       \
  RSP += 2 * sizeof(int64_t);        






int virtual_machine_init(virtual_machine_t* virtual_machine)
  {
    virtual_machine -> RAM = (unsigned char*)calloc(VIRTUAL_MACHINE_RAM_SIZE, sizeof(unsigned char));
    
    if ( virtual_machine -> RAM )
      return 0;
    else 
      return onError(MEMORY_ALLOCATION_ERROR);
  }



int virtual_machine_load(virtual_machine_t* virtual_machine, const unsigned char* input_file_name)
  {
    if (!input_file_name)
      onError(INAPPROPRIATE_INPUT_FILE_NAME_ERROR);

    return loadToRAM(virtual_machine, input_file_name);  
  }


int virtual_machine_run(virtual_machine_t* virtual_machine)
  {
    virtual_machine -> REGISTER.IP = 0;
    virtual_machine -> REGISTER.SP = VIRTUAL_MACHINE_RAM_SIZE - 1;
    
    const int TERMINATION_DIR = 0xFF;

    #define VRAM virtual_machine -> RAM
    #define RAX virtual_machine -> REGISTER.AX
    #define RBX virtual_machine -> REGISTER.BX
    #define RCX virtual_machine -> REGISTER.CX
    #define RDX virtual_machine -> REGISTER.DX
    #define RBP virtual_machine -> REGISTER.BP
    #define RSP virtual_machine -> REGISTER.SP
    #define RIP virtual_machine -> REGISTER.IP
    
    int64_t input_number = 0;
  
    while ( virtual_machine -> RAM[virtual_machine -> REGISTER.IP] != TERMINATION_DIR )
    {
      switch(virtual_machine -> RAM[virtual_machine -> REGISTER.IP++] )
        {
          #define DEFINE_COMMAND(name, num, code)    \
            case num:                                \
              code                                   \                
              break;

          #include "Java Playgrnd Virtual Machine Commands.h"

            default:
              return onError(UNKNOWN_COMMAND_ERROR);
              break;

         #undef DEFINE_COMMAND                                          
       }
    }
    if ( virtual_machine -> RAM[virtual_machine -> REGISTER.IP] == TERMINATION_DIR )
      return onError(PROGRAM_TERMINATION_ERROR);

    #undef VRAM
    #undef RAX  
    #undef RBX
    #undef RCX
    #undef RDX
    #undef RBP
    #undef RSP
    #undef RIP
      
  }


int virtual_machine_stop(virtual_machine_t* virtual_machine)
  {
    free(virtual_machine -> RAM);
    virtual_machine -> RAM = NULL;
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


int64_t getFromRegister(virtual_machine_t* virtual_machine, unsigned char reg)
  {
    switch(reg)
      {
        #define DEFINE_REGISTER(name, num)            \
          case num:                                   \
            return virtual_machine -> REGISTER.name;  \ 
            break;
        
        #include "Java Playgrnd Virtual Machine Registers.h"
          
          default:
            onError(UNKNOWN_REGISTER_ERROR);
            break;
         
        #undef DEFINE_REGISTER
      }
  }







int loadToRAM(virtual_machine_t* virtual_machine, const unsigned char* input_file_name)
  {
    if ( !virtual_machine -> RAM)
      return onError(NO_INITILIZATION_ERROR);

    FILE* input_file = fopen(input_file_name, "rb");
    if ( !input_file )
      return onError(CANNOT_OPEN_INPUT_FILE_ERROR);

    size_t input_buffer_length = 0;
    unsigned char* input_buffer = NULL;
    if ( !(input_buffer = getInputBuffer(input_file, &input_buffer_length)) ) 
      return onError(CANNOT_READ_INPUT_FILE_ERROR);
      
    size_t header_length = readJPLHeader(input_buffer, input_buffer_length);  
    copyToRAM(virtual_machine, input_buffer + header_length, input_buffer_length - header_length);

    free(input_buffer);
    fclose(input_file);

    return 0;
  }




size_t readJPLHeader(unsigned char* input_buffer, size_t input_buffer_length)
  {
    const size_t header_length = 16;
    
    if ( input_buffer_length < header_length)
      onError(BAD_JPL_ERROR);

    return header_length;
  }


void copyToRAM(virtual_machine_t* virtual_machine, const unsigned char* input_buffer, size_t input_buffer_length)
  {
    size_t cnt = 0;
    for ( cnt = 0; cnt < input_buffer_length; cnt++)
      virtual_machine -> RAM[cnt] = *(unsigned char*)(input_buffer + cnt);
  }




int onError(int error_code)
  {
    switch (error_code)
      {
        #define DEFINE_ERROR(error_name, code, message) \
          case code:                                    \
            fprintf(stderr, "%s\n", message);             \
            exit(code);                                   \
            break;

        #include "Java Playgrnd Virtual Machine Errors.h"
          default:
            fprintf(stderr, "UNKNOWN ERRROR");
            exit(1);
            break; 

        #undef DEFINE_ERROR  
      }

    return error_code;  
  }  


