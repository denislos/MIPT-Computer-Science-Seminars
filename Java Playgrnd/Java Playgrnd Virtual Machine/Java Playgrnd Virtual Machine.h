
#ifndef JAVA_PLAYGRND_VIRTUAL_MACHINE
#define JAVA_PLAYGRND_VIRTUAL_MACHINE

#define VIRTUAL_MACHINE_RAM_SIZE 0x0FFFFFFF

#include <stdlib.h>


typedef struct virtual_machine_register_t
  {
    #define DEFINE_REGISTER(name, num) \
      int64_t name;
    
    #include "Java Playgrnd Virtual Machine Registers.h"

    #undef DEFINE_REGISTER
  } virtual_machine_register_t;


typedef struct virtual_machine_t
  {
    virtual_machine_register_t REGISTER;
    unsigned char* RAM;
  } virtual_machine_t;



int virtual_machine_init(virtual_machine_t* virtual_machine);

int virtual_machine_load(virtual_machine_t* virtual_machine, const unsigned char* input_file_name);

int virtual_machine_run(virtual_machine_t* virtual_machine);

int virtual_machine_stop(virtual_machine_t* virtual_machine);



#endif  