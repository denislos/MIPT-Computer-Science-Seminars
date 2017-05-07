
#ifndef MY_LIST_H
#define MY_LIST_H


typedef struct list_node_t
  {
    void* elem;
    struct list_node_t* next;
    struct list_node_t* prev;
  } list_node_t;

typedef struct list_t
  {
    list_node_t* head;
    list_node_t* tail;
  } list_t;



void list_ctor(list_t* list);

void list_dtor(list_t* list, int (*list_elem_dtor)(void* ) );

void list_push_head(list_t* list, void* elem);

void list_push_tail(list_t* list, void* elem);

void* list_pop_head(list_t* list);

void* list_pop_tail(list_t* list);

list_node_t* list_find(list_t* list, void* elem, int (*compare)(void* , void* ) );

void list_sort(list_t* list, int (*compare)(void* , void* ) );



#endif // MY_LIST_H