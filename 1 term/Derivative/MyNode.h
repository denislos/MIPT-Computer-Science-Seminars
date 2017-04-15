

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>


const unsigned long LEFT_CANARY_VALUE = 0xDFDFDF;
const unsigned long RIGHT_CANARY_VALUE = 0xABDFFBB;
const int MAX_STRING_LENGTH = 512;
const char* STANDART_DUMP_FILE = "NodeDumpFile.gv";


#define DEF_NODE_STANDART_TYPES \
  DEF_NODE_TYPE(int, int) \
  DEF_NODE_TYPE(char, char) \
  DEF_NODE_TYPE(double, double)



#define node_t(type) node_##type##_t


#define DEF_NODE_TYPE(elem_type, def_elem_t ) \
  typedef def_elem_t elem_##elem_type##_t;

DEF_NODE_STANDART_TYPES
DEF_NODE_TYPE(string, char*)

#undef DEF_NODE_TYPE




#define DEF_NODE_TYPE(elem_type, def_elem_t) \
  typedef struct node_item_##elem_type##_t \
    { \
      elem_##elem_type##_t elem; \
      struct node_item_##elem_type##_t* prev; \
      struct node_item_##elem_type##_t* next;  \
    } node_item_##elem_type##_t;

DEF_NODE_STANDART_TYPES
DEF_NODE_TYPE(string, char*)

#undef DEF_NODE_TYPE




#define DEF_NODE_TYPE(elem_type, def_elem_t) \
  typedef struct node_##elem_type##_t \
  {                                    \
    unsigned long left_canary;          \
    node_item_##elem_type##_t* head;     \
    node_item_##elem_type##_t* current;   \
    node_item_##elem_type##_t* tail;       \
    unsigned long right_canary;             \
  } node_##elem_type##_t;

DEF_NODE_STANDART_TYPES
DEF_NODE_TYPE(string, char*)

#undef DEF_NODE_TYPE



#define Object_IsOK(Object, Object_t) \
  {\
    assert(Object); \
    if ( Object_t##_IsOK(Object) == 0 ) \
      {                        \
        Object_t##_Dump(Object, STANDART_DUMP_FILE); \
        assert(0);                                  \
      }                                              \
  }



#define DEF_NODE_TYPE(elem_type, def_elem_t)                                                            \
  int Node_IsOK(node_##elem_type##_t* Node)                                                             \
    {                                                                                                   \
      assert(Node);                                                                                     \
      return ( ( (Node -> head && Node -> current && Node -> tail) || ( Node -> head == NULL && Node -> current == NULL && Node -> tail == NULL ) ) &&  Node -> right_canary == RIGHT_CANARY_VALUE && Node -> left_canary == LEFT_CANARY_VALUE ); \
    }

DEF_NODE_STANDART_TYPES
DEF_NODE_TYPE(string, char*)

#undef DEF_NODE_TYPE




int print_elem(FILE* dump_file, char elem)
  {
    assert(dump_file);
    fprintf(dump_file, "%c", elem);
    return 0;
  }

int print_elem(FILE* dump_file, int elem)
  {
    assert(dump_file);
    fprintf(dump_file, "%d", elem);
    return 0;
  }

int print_elem(FILE* dump_file, double elem)
  {
    assert(dump_file);
    fprintf(dump_file, "%lg", elem);
    return 0;
  }

int print_elem(FILE* dump_file, char* elem )
  {
    assert(dump_file);
    fprintf(dump_file, "%s", elem );
    return 0;
  }



#define DEF_NODE_TYPE(elem_type, def_elem_t) \
  int Node_MainDump(node_##elem_type##_t* Node, FILE* dump_file,  unsigned long node_count)   \
    {                                          \
      assert(Node);     \
      assert(dump_file); \
      unsigned long item_count = 0;                        \
      node_item_##elem_type##_t* p = Node -> head;                                                            \
      if ( p != NULL )                                                                                        \
        {                                                                                                     \
          fprintf(dump_file, "node_struct%d_item0[label = \"{<prev> prev | <elem>", node_count);                                        \
          print_elem(dump_file, Node -> head -> elem);                                                        \
          fprintf(dump_file, "| <next> next\}\"];\n");                                                         \
          p = p -> next;                                                                                       \
          item_count++;                                                                                        \
          if ( Node -> current == Node -> head ) fprintf(dump_file, "node_struct%d:current -> node_struct%d_item0;\n", node_count, node_count); \
          else if ( Node -> current != NULL)                             \
            {                                       \
              while ( p != Node -> current )                                                                                    \
                {                                                                                                     \
                  fprintf(dump_file, "node_struct%d_item%d[label = \"{<prev> prev | <elem>", node_count, item_count);                            \
                  print_elem(dump_file, p -> elem);                                                                     \
                  fprintf(dump_file, "| <next> next\}\"];\n");                                                            \
                  fprintf(dump_file, "node_struct%d_item%d:prev -> node_struct%d_item%d;\nnode_struct%d_item%d:next -> node_struct%d_item%d;\n",node_count, item_count, node_count,  item_count - 1, node_count,  item_count - 1, node_count,  item_count); \
                  item_count++;                                                                                                        \
                  p = p -> next;                                                                                                        \
                }                                                                                                                        \
              fprintf(dump_file, "node_struct%d_item%d[label = \"{<prev> prev | <elem>", node_count, item_count);                            \
              print_elem(dump_file, p -> elem);                                                                     \
              fprintf(dump_file, "| <next> next\}\"];\n");                                                            \
              fprintf(dump_file, "node_struct%d_item%d:prev -> node_struct%d_item%d;\nnode_struct%d_item%d:next -> node_struct%d_item%d;\n", node_count, item_count, node_count, item_count - 1, node_count,  item_count - 1, node_count, item_count); \
              p = p -> next;                                                                                    \
              fprintf(dump_file, "node_struct%d:current -> node_struct%d_item%d;\n",node_count, node_count,  item_count++);                              \
             }                                                                                                    \
          while ( p != NULL )  \
            {                   \
              fprintf(dump_file, "node_struct%d_item%d[label = \"{<prev> prev | <elem>", node_count,  item_count);                            \
              print_elem(dump_file, p -> elem);                                                                     \
              fprintf(dump_file, "| <next> next\}\"];\n");                                                            \
              fprintf(dump_file, "node_struct%d_item%d:prev -> node_struct%d_item%d;\nnode_struct%d_item%d:next -> node_struct%d_item%d;\n", node_count, item_count, node_count,  item_count - 1, node_count,  item_count - 1, node_count, item_count); \
              item_count++;                                                                                                        \
              p = p -> next;                                                                                                         \
            }                                                                                                                        \
          fprintf(dump_file, "node_struct%d:head -> node_struct%d_item0;\nnode_struct%d:tail -> node_struct%d_item%d;\n", node_count, node_count, node_count, node_count,  item_count -1 ); \
        }                                                                                                     \
    }

DEF_NODE_STANDART_TYPES
DEF_NODE_TYPE(string, char*)

#undef DEF_NODE_TYPE





#define DEF_NODE_TYPE(elem_type, def_elem_t)      \
  int Node_Dump(node_##elem_type##_t* Node, const char* DUMP_FILE_NAME)  \
    {                                                                     \
      assert(Node);                                                        \
      assert(DUMP_FILE_NAME);                                               \
      FILE* dump_file = fopen(DUMP_FILE_NAME, "w");                          \
      if ( dump_file == NULL ) return 1;                                      \
      fprintf(dump_file, "digraph Node_Dump {\n node[shape = record];\n"         \
                         "node_struct0[label = \"{<head> head | <current> current | <tail> tail} \"];\n");    \
      Node_MainDump(Node, dump_file, 0);                                         \
      fprintf(dump_file, "\n}");                                                \
      fclose(dump_file); dump_file = NULL;                                                                              \
      return 0;      \
    }

DEF_NODE_STANDART_TYPES
DEF_NODE_TYPE(string, char*)

#undef DEF_NODE_TYPE




#define DEF_NODE_TYPE(elem_type, def_elem_t) \
  int Node_ctor( node_##elem_type##_t* Node) \
    {                           \
      assert(Node);              \
      Node -> left_canary = LEFT_CANARY_VALUE; \
      Node -> right_canary = RIGHT_CANARY_VALUE; \
      Node -> tail = Node -> current = Node -> head = NULL; \
      Object_IsOK(Node, Node); \
      return 0;                                              \
    }

DEF_NODE_STANDART_TYPES
DEF_NODE_TYPE(string, char*)

#undef DEF_NODE_TYPE




#define DEF_NODE_TYPE(elem_type, def_elem_t) \
  int Node_dtor(node_##elem_type##_t* Node)           \
  {                                                   \
    assert(Node);                                     \
    Node -> left_canary = Node -> right_canary = 0;   \
    node_item_##elem_type##_t* p = Node -> head;   \
    node_item_##elem_type##_t* q = NULL;          \
    while ( p != NULL )            \
      {                           \
        q = p -> next;           \
        free(p);                \
        p = q;                 \
      }                       \
    Node -> head = NULL;     \
    Node -> current = NULL; \
    Node -> tail = NULL; \
    return 0;    \
  }

DEF_NODE_STANDART_TYPES

#undef DEF_NODE_TYPE





#define DEF_NODE_TYPE(elem_type, def_elem_t) \
  int Node_IncCurrent(node_##elem_type##_t* Node) \
    {                                                \
      assert(Node);                                   \
      Object_IsOK(Node, Node);                         \
      if ( Node -> current == Node -> tail ) return 1;  \
      else Node -> current = Node -> current -> next;    \
      Object_IsOK(Node, Node);                            \
      return 0;                                            \
    }

DEF_NODE_STANDART_TYPES
DEF_NODE_TYPE(string, char*)

#undef DEF_NODE_TYPE



#define DEF_NODE_TYPE(elem_type, def_elem_t) \
  int Node_StdInc(node_##elem_type##_t* Node) \
    {                                             \
    	assert(Node);                                 \
    	Node -> current = Node -> current -> next;    \
    	return 0;                                     \
    }

DEF_NODE_STANDART_TYPES
DEF_NODE_TYPE(string, char*)

#undef DEF_NODE_TYPE



#define DEF_NODE_TYPE(elem_type, def_elem_t)         \
  int Node_DecCurrent(node_##elem_type##_t* Node)    \
    {                                                 \
      assert(Node);                                    \
      Object_IsOK(Node, Node);                         \
      if ( Node -> current == Node -> head ) return 1;  \
      else Node -> current = Node -> current -> prev;    \
      Object_IsOK(Node, Node);                            \
      return 0;                                            \
    }

DEF_NODE_STANDART_TYPES
DEF_NODE_TYPE(string, char*)

#undef DEF_NODE_TYPE




#define DEF_NODE_TYPE(elem_type, def_elem_t) \
int Node_TailPush( node_##elem_type##_t* Node, elem_##elem_type##_t elem)    \
  {                                   \
    assert(Node);                      \
    Object_IsOK(Node, Node); \
    if ( Node -> head == NULL )         \
      {                                  \
        Node -> tail = Node -> current = Node -> head = ( node_item_##elem_type##_t*) calloc(1, sizeof(*(Node -> head))); \
        Node -> head -> elem = elem;                                                                                       \
        Node -> head -> next = Node -> head -> prev = NULL;                                                                 \
      }                                                                                                                      \
    else                                                                                                                      \
      {                                                                                                                     \
        node_item_##elem_type##_t* new_item = ( node_item_##elem_type##_t* ) calloc(1, sizeof(*(Node -> tail)));         \
        new_item -> elem = elem;                                                                                          \
        new_item -> next = NULL;                                                                                           \
        Node -> tail -> next = new_item;                                                                                  \
        new_item -> prev = Node -> tail;                                                                                   \
        Node -> tail = new_item;                                                                                          \
      }                                                                                                                 \
    Object_IsOK(Node, Node);                                                                                         \
    return 0;                                                                                                             \
  }                                                                                                                       \

DEF_NODE_STANDART_TYPES

#undef DEF_NODE_TYPE





#define DEF_NODE_TYPE(elem_type, def_elem_t) \
int Node_HeadPush(node_##elem_type##_t* Node, elem_##elem_type##_t elem) \
  {                                                               \
    assert(Node);                                            \
    Object_IsOK(Node, Node); \
    if ( Node -> head == NULL)                               \
      {                                                             \
        Node -> head = Node -> current = Node -> tail = (node_item_##elem_type##_t*) calloc(1, sizeof(*Node -> head)); \
        Node -> head -> elem = elem;                                                                                    \
        Node -> head -> prev = Node -> head -> next = NULL;                                                              \
      }    \
    else    \
      {                                                                                                   \
        node_item_##elem_type##_t* new_item = (node_item_##elem_type##_t*) calloc(1, sizeof(*new_item));    \
        new_item -> elem = elem;                                                                         \
        new_item -> prev = NULL;                                                                         \
        new_item -> next = Node -> head;                                                                 \
        Node -> head -> prev = new_item;                                                                 \
        Node -> head = new_item;                                                                         \
      }                                                                                                   \
    Object_IsOK(Node, Node);   \
    return 0;                                                                                             \
  }                                                                                                       \

DEF_NODE_STANDART_TYPES

#undef DEF_NODE_TYPE





#define DEF_NODE_TYPE(elem_type, def_elem_t) \
int Node_CurrentPush_After( node_##elem_type##_t* Node, elem_##elem_type##_t elem)  \
  {                                                                                 \
    assert(Node);                                                                    \
    Object_IsOK(Node, Node); \
    if ( Node -> head == NULL )                                                       \
      {                                                                                \
        Node -> tail = Node -> current = Node -> head = (node_item_##elem_type##_t*) calloc(1, sizeof(*Node -> head ) ); \
        Node -> head -> elem = elem;                                                                                     \
        Node -> head -> prev = Node -> head -> next = NULL;                                                              \
      }                                                                                                                   \
    else                                                                                                                  \
      {                                                                                                                   \
        node_item_##elem_type##_t* new_item = (node_item_##elem_type##_t*) calloc(1, sizeof(*new_item));  \
        new_item -> elem = elem;                                                                           \
        new_item -> prev = Node -> current;                                                                \
        new_item -> next = Node -> current -> next;                                                        \
        Node -> current -> next = new_item;                                                                \
        if ( Node -> current == Node -> tail ) Node -> tail = new_item;                                    \
      }                                                                                                     \
    Object_IsOK(Node, Node); \
    return 0;                                                                                               \
  }

DEF_NODE_STANDART_TYPES

#undef DEF_NODE_TYPE




#define DEF_NODE_TYPE(elem_type, def_elem_t) \
int Node_CurrentPush_Before( node_##elem_type##_t* Node, elem_##elem_type##_t elem) \
  {                                                                                 \
    assert(Node);                                                                    \
    Object_IsOK(Node, Node);                                                          \
    if ( Node -> head == NULL )                                                       \
      {                                                                                \
        Node -> tail = Node -> current = Node -> head = ( node_item_##elem_type##_t*) calloc(1, sizeof(*Node -> head));  \
        Node -> head -> elem = elem;                                                                                     \
        Node -> head -> prev = Node -> head -> next = NULL;                                                              \
      }                                                                                                                   \
    else                                                                                                     \
      {                                                                                                       \
        node_item_##elem_type##_t* new_item = (node_item_##elem_type##_t* ) calloc(1, sizeof(*new_item));     \
        new_item -> elem = elem;                                                                              \
        new_item -> next = Node -> current;                                                                    \
        new_item -> prev = Node -> current -> prev;                                                             \
        Node -> current -> prev = new_item;                                                                     \
        if ( Node -> current == Node -> head ) Node -> head = new_item;                                         \
      }                                                                                                          \
    Object_IsOK(Node, Node); \
    return 0;                                                                                                    \
  }                                                                                                               \

DEF_NODE_STANDART_TYPES

#undef DEF_NODE_TYPE





#define DEF_NODE_TYPE(elem_type, def_elem_t) \
elem_##elem_type##_t Node_TailPop(node_##elem_type##_t* Node) \
  {                                           \
    assert(Node);                              \
    Object_IsOK(Node, Node); \
    assert(Node -> tail);                       \
    elem_##elem_type##_t x = Node -> tail -> elem;                \
    if ( Node -> tail == Node -> head )           \
      {                                            \
        free(Node -> tail);                         \
        Node -> tail = NULL;                       \
        Node -> head = NULL;                       \
        Node -> current = NULL;                    \
      }                                             \
    else                                            \
      {                                              \
        node_item_##elem_type##_t* q = Node -> tail -> prev;   \
        q -> next = NULL;                                       \
        if ( Node -> current == Node -> tail  ) Node -> current = q;  \
        free( Node -> tail );                                          \
        Node -> tail = q;                                               \
      }                                                                 \
    Object_IsOK(Node, Node); \
    return x;                                                            \
  }

DEF_NODE_STANDART_TYPES

#undef DEF_NODE_TYPE




#define DEF_NODE_TYPE(elem_type, def_elem_t) \
elem_##elem_type##_t Node_HeadPop(node_##elem_type##_t* Node) \
  {                                           \
    assert(Node);                              \
    assert(Node -> head);                       \
    Object_IsOK(Node, Node); \
    elem_##elem_type##_t x = Node -> head -> elem;                \
    if ( Node -> head == Node -> tail )           \
      {                                           \
        free(Node -> head);                        \
        Node -> head = NULL;                        \
        Node -> tail = NULL;                        \
        Node -> current = NULL;                      \
      }                                              \
    else                                             \
      {                                               \
        node_item_##elem_type##_t* q = Node -> head -> next;   \
        q -> prev = NULL;                                       \
        if ( Node -> current = Node -> head ) Node -> current = q;  \
        free( Node -> head);                                         \
        Node -> head = q;                                             \
      }                                                                \
    Object_IsOK(Node, Node);    \
    return x;                                                        \
  }


DEF_NODE_STANDART_TYPES

#undef DEF_NODE_TYPE




#define DEF_NODE_TYPE(elem_type, def_elem_t) \
elem_##elem_type##_t Node_CurrentPop(node_##elem_type##_t* Node)                       \
  {                                                                   \
    assert(Node);                                                     \
    Object_IsOK(Node, Node); \
    assert(Node -> current);                                          \
    elem_##elem_type##_t x  = Node -> current -> elem;                                 \
    if ( Node -> head == Node -> tail )                               \
      {                                                               \
        free(Node -> current);                                        \
        Node -> head = NULL;                                          \
        Node -> current = NULL;                                       \
        Node -> tail = NULL;                                          \
      }                                                               \
    else if ( Node -> current == Node -> tail )                       \
      {                                                               \
        node_item_##elem_type##_t* q = Node -> current -> prev;       \
        q -> next = NULL;                                             \
        free(Node -> current);                                        \
        Node -> tail = Node -> current = q;                           \
      }                                                               \
    else if ( Node -> current == Node -> head )                       \
      {                                                               \
        node_item_##elem_type##_t* q = Node -> current -> next;       \
        q -> prev = NULL;                                             \
        free(Node -> current);                                        \
        Node -> head = Node -> current = q;                           \
      }                                                               \
    else                                                              \
      {                                                               \
        Node -> current -> prev -> next = Node -> current -> next;    \
        Node -> current -> next -> prev = Node -> current -> prev;    \
        node_item_##elem_type##_t* q = Node -> current -> prev;       \
        free(Node -> current);        \
        Node -> current = q;          \
      }                               \
    Object_IsOK(Node, Node); \
    return x;                         \
  }

DEF_NODE_STANDART_TYPES

#undef DEF_NODE_TYPE




#define DEF_NODE_TYPE(elem_type, def_elem_t) \
  node_item_##elem_type##_t* Node_LineSearch(node_##elem_type##_t* Node, elem_##elem_type##_t elem) \
    {                                                                                                \
      assert(Node);                                                                                   \
      Object_IsOK(Node, Node);                                                                         \
      node_item_##elem_type##_t* p = Node -> head;                                                       \
      while ( p != NULL )                                                                                \
        if ( p -> elem == elem ) return p;                                                                \
        else p = p -> next;                                                                                \
      return NULL;                                                                                          \
    }

DEF_NODE_STANDART_TYPES

#undef DEF_NODE_TYPE



#define DEF_NODE_TYPE(elem_type, def_elem_t) \
  unsigned long Node_ElemCount(node_##elem_type##_t* Node)      \
    {                                                                                       \
    	assert(Node);                                                                         \
    	Object_IsOK(Node, Node);   \
    	node_item_##elem_type##_t* p = Node -> head;   \
    	unsigned long elem_count = 0;                  \
    	while ( p != NULL )                            \
			  {                                            \
			  	elem_count++;                              \
			  	p = p -> next;                             \
			  }                                            \
			return elem_count;                             \
    }

DEF_NODE_STANDART_TYPES
DEF_NODE_TYPE(string, char*)

#undef DEF_NODE_TYPE



// node_string_t

int Node_dtor(node_string_t* Node)
  {
    assert(Node);
    Node -> left_canary = Node -> right_canary = 0;
    node_item_string_t* p = Node -> head;
    node_item_string_t* q = NULL;
    while ( p != NULL )
      {
        q = p -> next;
        free(p -> elem);
        p -> elem = NULL;
        free(p);
        p = q;
      }
    Node -> head = NULL;
    Node -> current = NULL;
    Node -> tail = NULL;
    return 0;
  }



int Node_TailPush( node_string_t* Node, elem_string_t elem)
  {
    assert(Node);
    assert(elem);
    Object_IsOK(Node, Node);
    if ( Node -> head == NULL )
      {
        Node -> tail = Node -> current = Node -> head = ( node_item_string_t*) calloc(1, sizeof(*(Node -> head)));
        Node -> head -> elem = ( elem_string_t ) calloc(MAX_STRING_LENGTH, sizeof(char));
        strncpy(Node -> head -> elem , elem, MAX_STRING_LENGTH - 1);
        Node -> head -> next = Node -> head -> prev = NULL;
      }
    else
      {
        node_item_string_t* new_item = ( node_item_string_t* ) calloc(1, sizeof(*(Node -> tail)));
        new_item -> elem = (elem_string_t ) calloc(MAX_STRING_LENGTH, sizeof(char));
        strncpy(new_item -> elem , elem, MAX_STRING_LENGTH - 1);
        new_item -> next = NULL;
        Node -> tail -> next = new_item;
        new_item -> prev = Node -> tail;
        Node -> tail = new_item;
      }
    Object_IsOK(Node, Node);
    return 0;
  }


int Node_HeadPush(node_string_t* Node, elem_string_t elem)
  {
    assert(Node);
    Object_IsOK(Node, Node); \
    if ( Node -> head == NULL)
      {
        Node -> head = Node -> current = Node -> tail = (node_item_string_t*) calloc(1, sizeof(*Node -> head));
        Node -> head -> elem = ( elem_string_t) calloc(MAX_STRING_LENGTH, sizeof(char));
        strncpy(Node -> head -> elem , elem, MAX_STRING_LENGTH - 1);
        Node -> head -> prev = Node -> head -> next = NULL;
      }
    else
      {
        node_item_string_t* new_item = (node_item_string_t*) calloc(1, sizeof(*new_item));
        new_item -> elem = ( elem_string_t) calloc(MAX_STRING_LENGTH, sizeof(char));
        strncpy(new_item -> elem , elem, MAX_STRING_LENGTH - 1);
        new_item -> prev = NULL;
        new_item -> next = Node -> head;
        Node -> head -> prev = new_item;
        Node -> head = new_item;
      }
    Object_IsOK(Node, Node);
    return 0;
  }


elem_string_t Node_HeadPop(node_string_t* Node)
  {
    assert(Node);
    assert(Node -> head);
    Object_IsOK(Node, Node);
    elem_string_t x = ( elem_string_t) calloc(MAX_STRING_LENGTH, sizeof(char));
    strncpy(x, Node -> head -> elem, MAX_STRING_LENGTH);
    if ( Node -> head == Node -> tail )
      {
        free(Node -> head -> elem);
        free(Node -> head);
        Node -> head = NULL;
        Node -> tail = NULL;
        Node -> current = NULL;
      }
    else
      {
        node_item_string_t* q = Node -> head -> next;
        q -> prev = NULL;
        if ( Node -> current = Node -> head ) Node -> current = q;
        free(Node -> head -> elem);
        free( Node -> head);
        Node -> head = q;
      }
    Object_IsOK(Node, Node);
    return x;
  }

node_item_string_t* Node_LineSearch(node_string_t* Node, elem_string_t elem )
  {
    assert(Node);
    assert(elem);
    Object_IsOK(Node,Node);
    node_item_string_t* p = Node -> head;
    while ( p != NULL )
      if ( strncmp(p -> elem, elem, MAX_STRING_LENGTH) == 0 ) return p;
      else p = p -> next;
    return NULL;
  }



