

#include "Java Playgrnd Virtual Machine.h"


int main(int argc, char** argv)
  {

    virtual_machine_t jpl_virtual_machine;
    virtual_machine_init(&jpl_virtual_machine);

    virtual_machine_load(&jpl_virtual_machine, argv[1]);

    virtual_machine_run(&jpl_virtual_machine);  
    
    virtual_machine_stop(&jpl_virtual_machine);
    
    return 0;
  }