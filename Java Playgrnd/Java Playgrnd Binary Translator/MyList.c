

#include <stdlib.h>
#include <assert.h>

#include "MyList.h"



void list_ctor(list_t* list)
  {
    assert(list);
    
    list -> head = list -> tail = NULL;
  }




void list_dtor(list_t* list, int (*list_elem_dtor)(void* ) )
  {
    assert(list);

    while ( list -> head )
      {
        list_node_t* next_node = list -> head -> next;
        list_elem_dtor(list -> head -> elem);
        free(list -> head);
        list -> head = next_node;
      }  
  }  





void list_push_head(list_t* list, void* elem)
  {
    assert(list);

    if ( !(list -> head ) )
      {
        list -> tail = list -> head  = ( list_node_t* )calloc(1, sizeof(list_node_t));
        list -> head -> elem = elem;
        list -> head -> next = list -> head -> prev = NULL;
      }
    else
      {
        list_node_t* new_node = ( list_node_t* ) calloc(1, sizeof(*new_node));
        
        new_node -> next  = list -> head;
        new_node -> prev  = NULL;
        new_node -> elem = elem;

        list -> head -> prev  = new_node;
        list -> head = new_node; 
      }
    
  }




void list_push_tail(list_t* list, void* elem)
  {
    assert(list);

    if ( !(list -> tail) )
      {
        list -> head = list -> tail = ( list_node_t* )calloc(1, sizeof(list_node_t) );
        list -> tail -> elem = elem;
        list -> tail -> prev = list -> tail -> next = NULL;      
      }
    else
      {
        list_node_t* new_node = ( list_node_t*) calloc(1, sizeof(*new_node));

        new_node -> next = NULL;
        new_node -> prev = list -> tail;
        new_node -> elem = elem;

        list -> tail -> next = new_node;
        list -> tail = new_node;
      }
    
  }    




void* list_pop_head(list_t* list)
  {
    void* elem = NULL;

    if ( list -> head )
      {
        elem = list -> head -> elem;
        
        list_node_t* next_node = list -> head -> next;
        if ( !next_node ) 
          list -> tail = NULL;

        free(list -> head);
        list -> head = next_node;  
      }

    return elem;  
  }





void* list_pop_tail(list_t* list)
  {
    void* elem = NULL;

    if ( list -> tail )
      {
        elem = list -> tail -> elem;

        list_node_t* prev_node = list -> tail -> prev;
        if ( !prev_node )
          list -> head  = NULL;

        free(list -> tail);
        list -> tail = prev_node;  
      }
    
    return elem;
  }  





list_node_t* list_find(list_t* list, void* elem, int (*compare)(void* , void* ) )
  {
    assert(list);

    list_node_t* list_ptr = list -> head;
    while ( list_ptr )
      {

        if ( compare(list_ptr -> elem, elem) == 0 )
          return list_ptr;
        else
          list_ptr = list_ptr -> next;
      }
    
    return NULL;
  }



void list_sort(list_t* list, int (*compare)(void* , void* ) )
  {
    assert(list);

    list_node_t* i = NULL;
    list_node_t* j = NULL;

    for ( j = list -> tail; j != list -> head; j = j -> prev)
      {
        int isSorted = 1;
  
        for ( i = list -> head; i != j; i = i -> next )
          if ( compare(i -> elem, i -> next -> elem) > 0 )
            {
              isSorted = 0;

              void* tmp = i -> elem;
              i -> elem = i -> next -> elem;
              i -> next -> elem = tmp;
            }

        if ( isSorted )
          break;    
      }
  }

 



