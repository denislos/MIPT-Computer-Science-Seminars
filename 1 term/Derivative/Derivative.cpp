

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <io.h>
#include <math.h>
#include "C:\Users\Äåíèñ\Desktop\Projects\C Codeblocks Projects\NodeLib\MyNode.h"

const unsigned long TREE_DIFF_LEFT_CANARY = 0xDDDABAF;
const unsigned long TREE_DIFF_RIGHT_CANARY = 0xDDBFABA;
const unsigned long NODE_DIFF_LEFT_CANARY = 0xBDBBAFF;
const unsigned long NODE_DIFF_RIGHT_CANARY = 0xDFFDBDBA;
const unsigned int MAX_TEX_DUMP_SIZE = 4000;
const int MAX_EXPRESSION_STRING = 10;
const unsigned int MAX_TEXT_ARRAY_SIZE = 10;
const char* text_array[MAX_TEXT_ARRAY_SIZE] =
{
  "Actually, you have to know that the derivative of a function of a real variable measures the sensitivity to change of a quantity (a function value or dependent variable) which is determined by another quantity (the independent variable). \nDerivatives are a fundamental tool of calculus. For example, the derivative of the position of a moving object with respect to time is the object's velocity: this measures how quickly the position of the object changes when time is advanced.\n",
  "Most functions that occur in practice have derivatives at all points or at almost every point. Early in the history of calculus, many mathematicians assumed that a continuous function was differentiable at most points. Under mild conditions, for example if the function is a monotone function or a Lipschitz function, this is true. \nHowever, in 1872 Weierstrass found the first example of a function that is continuous everywhere but differentiable nowhere.\n",
  "Multiplication can also be visualized as counting objects arranged in a rectangle (for whole numbers) or as finding the area of a rectangle whose sides have given lengths. The area of a rectangle does not depend on which side is measured first, which illustrates the commutative property. The product of two measurements is a new type of measurement, for instance multiplying the lengths of the two sides of a rectangle gives its area, this is the subject of dimensional analysis.\n",
  "Multiplication is also defined for other types of numbers, such as complex numbers, and more abstract constructs, like matrices. For these more abstract constructs, the order that the operands are multiplied sometimes does matter. A listing of the many different kinds of products that are used in mathematics is given in the product (mathematics) page"
  "It is one of the most important results in differential calculus, as well as one of the most important theorems in mathematical analysis, and is useful in proving the fundamental theorem of calculus. The mean value theorem follows from a more specific statement of Rolle's theorem, and can be used to prove the most general statement of Taylor's theorem (with Lagrange form of the remainder term)."
  "Jean Dieudonne in his classic treatise Foundations of Modern Analysis discards the mean value theorem and replaces it by mean inequality as the proof is not constructive and one cannot find the mean value and in applications one only needs mean inequality. Serge Lang in Analysis I uses the mean value theorem, in integral form, as an instant reflex but this use requires the continuity of the derivative. If one uses the Henstock–Kurzweil integral one can have the mean value theorem in integral form without the additional assumption that derivative should be continuous as every derivative is Henstock–Kurzweil integrable."
  "Cats are similar in anatomy to the other felids, with a strong, flexible body, quick reflexes, sharp retractable claws, and teeth adapted to killing small prey. Cat senses fit a crepuscular and predatory ecological niche. Cats can hear sounds too faint or too high in frequency for human ears, such as those made by mice and other small animals. They can see in near darkness. Like most other mammals, cats have poorer color vision and a better sense of smell than humans. Cats, despite being solitary hunters, are a social species and cat communication includes the use of a variety of vocalizations (mewing, purring, trilling, hissing, growling, and grunting), as well as cat pheromones and types of cat-specific body language."
  "Geometry arose independently in a number of early cultures as a practical way for dealing with lengths, areas, and volumes. Geometry began to see elements of formal mathematical science emerging in the West as early as the 6th century BC."
  "Proclus introduces Euclid only briefly in his Commentary on the Elements. According to Proclus, Euclid belonged to Plato's \"persuasion\" and brought together the Elements, drawing on prior work by several pupils of Plato (particularly Eudoxus of Cnidus, Theaetetus and Philip of Opus.) Proclus believes that Euclid is not much younger than these, and that he must have lived during the time of Ptolemy I because he was mentioned by Archimedes (287–212 BC). Although the apparent citation of Euclid by Archimedes has been judged to be an interpolation by later editors of his works, it is still believed that Euclid wrote his works before those of Archimedes."
  "Euler's formal education started in Basel, where he was sent to live with his maternal grandmother. In 1720, aged thirteen, he enrolled at the University of Basel, and in 1723, he received a Master of Philosophy with a dissertation that compared the philosophies of Descartes and Newton. During that time, he was receiving Saturday afternoon lessons from Johann Bernoulli, who quickly discovered his new pupil's incredible talent for mathematics"
};


#define tree_t(tree_type) tree_##tree_type##_t


enum tree_item_diff_type
  {
    #define DEF_ITEM_TYPE(item_type, num, sign, diff_code, simp_code, tex_dump_code) \
      item_type = num,
    #include "C:\Users\Äåíèñ\Desktop\Projects\C Codeblocks Projects\DiffTree\Tree_item_diff_types.h"
    NUMBER = 0,
    VARIABLE = 1
    #undef DEF_ITEM_TYPE
  };



typedef struct tree_item_diff_t
  {
    tree_item_diff_type item_type;
    double number;
    struct tree_item_diff_t* left;
    struct tree_item_diff_t* right;
  } tree_item_diff_t;




typedef struct tree_diff_t
  {
    unsigned long tree_left_canary;
    tree_item_diff_t* root;
    unsigned long tree_right_canary;
  } tree_diff_t;


typedef struct node_item_diff_t
  {
    tree_item_diff_t* elem;
    struct node_item_diff_t* next;
    struct node_item_diff_t* prev;
  } node_item_diff_t;


typedef struct node_diff_t
  {
    unsigned long node_left_canary;
    struct node_item_diff_t* head;
    struct node_item_diff_t* tail;
    unsigned long node_right_canary;
  } node_diff_t;


int Node_ctor(node_diff_t* Node)
  {
    assert(Node);

    Node -> node_left_canary  = NODE_DIFF_LEFT_CANARY;
    Node -> node_right_canary = NODE_DIFF_RIGHT_CANARY;

    Node -> head = Node -> tail = Node -> current = NULL;

    return 0;
  }


int Node_dtor(node_diff_t* Node)
  {
    assert(Node);

    Node -> node_left_canary = Node -> node_right_canary = 0;
    Node -> current = Node -> head;

    while ( Node -> current != NULL )
      {
        node_item_diff_t* ptr = Node -> current -> next;
        free(Node -> current -> elem);
        free(Node -> current);
        Node -> current = ptr;
      }
    Node -> head = Node -> current = Node -> tail = NULL;

    return 0;
  }


int Node_TailPush( node_diff_t* Node, tree_item_diff_t* elem)
  {
    assert(Node);
    if ( Node -> head == NULL )
      {
        Node -> tail = Node -> current = Node -> head = ( node_item_diff_t*) calloc(1, sizeof(*(Node -> head)));
        Node -> head -> elem = elem;
        Node -> head -> next = Node -> head -> prev = NULL;
      }
    else
      {
        node_item_diff_t* new_item = ( node_item_diff_t* ) calloc(1, sizeof(*(Node -> tail)));
        new_item -> elem = elem;
        new_item -> next = NULL;
        Node -> tail -> next = new_item;
        new_item -> prev = Node -> tail;
        Node -> tail = new_item;
      }                                                                                                                                                                                                         \
    return 0;
  }

tree_item_diff_t* Node_TailPop(node_diff_t* Node)
  {
    assert(Node);
    assert(Node -> tail);

    tree_item_diff_t* x = Node -> tail -> elem;

    if ( Node -> tail == Node -> head )
      {
        free(Node -> tail -> elem);
        free(Node -> tail);

        Node -> tail    = NULL;
        Node -> head    = NULL;
        Node -> current = NULL;
      }
    else
      {
        node_item_diff_t* q = Node -> tail -> prev;
        q -> next = NULL;
        if ( Node -> current == Node -> tail  ) Node -> current = q;
        free(Node -> tail -> elem);
        free( Node -> tail );
        Node -> tail = q;
      }
    return x;
  }



int Tree_ctor(tree_diff_t* Tree)
  {
    assert(Tree);

    Tree -> tree_left_canary  = TREE_DIFF_LEFT_CANARY;
    Tree -> tree_right_canary = TREE_DIFF_RIGHT_CANARY;

    Tree -> root = NULL;

    return 0;
  }


tree_item_diff_t* Tree_CreateItem(tree_item_diff_type item_type, double number)
  {
    tree_item_diff_t* new_item = ( tree_item_diff_t* ) calloc(1, sizeof(*new_item));

    new_item -> item_type = item_type;
    new_item -> left = new_item -> right = NULL;
    new_item -> number = ( item_type == NUMBER ) ? number : 0;

    return new_item;
  }


tree_item_diff_t* NewTreeItem(tree_item_diff_type item_type, double number, tree_item_diff_t* left_item, tree_item_diff_t* right_item)
  {
    tree_item_diff_t* new_item = ( tree_item_diff_t* ) calloc(1, sizeof(*new_item));
    new_item -> item_type = item_type;
    new_item -> number = ( item_type == NUMBER) ? number : 0;
    new_item -> left = left_item;
    new_item -> right = right_item;
    return new_item;
  }


char* GetInputBuf(char* input_file_name)
  {
    assert(input_file_name);

    FILE* input_file = fopen(input_file_name, "r");
    assert(input_file);

    long file_length = filelength(fileno(input_file));

    char* input_buf = ( char* ) calloc( file_length + 1, sizeof(*input_buf));
    fread(input_buf, sizeof(*input_buf), file_length, input_file);

    return input_buf;
  }


tree_item_diff_t* ExprLevel(node_t(string)* Node);

tree_item_diff_t* AtomLevel(node_t(string)* Node);

tree_item_diff_t* SubAtomLevel(node_t(string)* Node);

tree_item_diff_t* QuarkLevel(node_t(string)* Node);



tree_item_diff_t* ExprLevel(node_t(string)* Node)
  {
    if ( Node -> current == NULL ) return NULL;

    tree_item_diff_t* res = ( tree_item_diff_t* ) calloc(1, sizeof(*res));
    res = AtomLevel(Node);

    while ( Node -> current != NULL && ( Node -> current -> elem[0] == '+' || Node -> current -> elem[0] == '-') )
      {
        if ( Node -> current -> elem[0] == '+' )
          {
            Node_StdInc(Node);
            res = NewTreeItem(ADD, 0, res, AtomLevel(Node));
          }
        else if ( Node -> current -> elem[0] == '-' )
          {
            Node_StdInc(Node);
            res = NewTreeItem(SUB, 0, res, AtomLevel(Node));
          }
      }

    return res;
  }


tree_item_diff_t* AtomLevel(node_string_t* Node)
  {
    if ( Node -> current == NULL) return NULL;

    tree_item_diff_t* res = ( tree_item_diff_t* ) calloc(1, sizeof(*res));
    res = SubAtomLevel(Node);
    while ( Node -> current != NULL && (Node -> current -> elem[0] == '*' || Node -> current -> elem[0] == '/') )
      {
        if ( Node -> current -> elem[0] == '*' )
          {
            Node_StdInc(Node);
            res = NewTreeItem(MUL, 0, res, SubAtomLevel(Node));
          }
        else if (  Node -> current -> elem[0] == '/' )
          {
            Node_StdInc(Node);
            res = NewTreeItem(DIV, 0, res, SubAtomLevel(Node));
          }
      }
    return res;
  }



tree_item_diff_t* SubAtomLevel(node_string_t* Node)
  {
    if ( Node -> current == NULL ) return NULL;
    tree_item_diff_t* res = ( tree_item_diff_t* ) calloc(1, sizeof(*res));
    if ( Node -> current -> elem[0] == '(' )
      {
        Node_StdInc(Node);
        res = ExprLevel(Node);
        assert(Node -> current -> elem[0] == ')');
        Node_StdInc(Node);
        if ( Node -> current != NULL && Node -> current -> elem[0] == '^' )
          {
            Node_StdInc(Node);
            assert(Node -> current -> elem[0] == '(');
            Node_StdInc(Node);
            res = NewTreeItem(POW, 0, res, ExprLevel(Node));
            assert(Node -> current -> elem[0] == ')');
            Node_StdInc(Node);
          }
      }
    else if ( Node -> current -> elem[0] >= '0' && Node -> current -> elem[0] <= '9' )
      {
        res = QuarkLevel(Node);
        Node_StdInc(Node);
      }
    else if ( Node -> current -> elem[0] == 'x' )
      {
        res = Tree_CreateItem(VARIABLE, 0);
        Node_StdInc(Node);
      }
    else if ( stricmp(Node -> current -> elem, "SIN") == 0 )
      {
        Node_StdInc(Node);
        assert(Node -> current -> elem[0] == '(');
        Node_StdInc(Node);
        res = NewTreeItem(SIN, 0, NULL, ExprLevel(Node));
        assert(Node -> current -> elem[0] == ')');
        Node_StdInc(Node);
      }
    else if ( stricmp(Node -> current -> elem, "COS") == 0 )
      {
        Node_StdInc(Node);
        assert(Node -> current -> elem[0] == '(');
        Node_StdInc(Node);
        res = NewTreeItem(COS, 0, NULL, ExprLevel(Node));
        assert(Node -> current -> elem[0] == ')');
        Node_StdInc(Node);
      }
    else if ( stricmp(Node -> current -> elem, "TG") == 0 )
      {
        Node_StdInc(Node);
        assert(Node -> current -> elem[0] == '(');
        Node_StdInc(Node);
        res = NewTreeItem(TG, 0, NULL, ExprLevel(Node));
        assert(Node -> current -> elem[0] == ')');
        Node_StdInc(Node);
      }
    else if ( stricmp(Node -> current -> elem, "CTG") == 0 )
      {
        Node_StdInc(Node);
        assert(Node -> current -> elem[0] == '(');
        Node_StdInc(Node);
        res = NewTreeItem(SIN, 0, NULL, ExprLevel(Node));
        assert(Node -> current -> elem[0] == ')');
        Node_StdInc(Node);
      }
    else if ( stricmp(Node -> current -> elem, "ARCSIN") == 0 )
      {
        Node_StdInc(Node);
        assert(Node -> current -> elem[0] == '(');
        Node_StdInc(Node);
        res = NewTreeItem(ARCSIN, 0, NULL, ExprLevel(Node));
        assert(Node -> current -> elem[0] == ')');
        Node_StdInc(Node);
      }
    else if ( stricmp(Node -> current -> elem, "ARCCOS") == 0 )
      {
        Node_StdInc(Node);
        assert(Node -> current -> elem[0] == '(');
        Node_StdInc(Node);
        res = NewTreeItem(ARCCOS, 0, NULL, ExprLevel(Node));
        assert(Node -> current -> elem[0] == ')');
        Node_StdInc(Node);
      }
    else if ( stricmp(Node -> current -> elem, "ARCTG") == 0 )
      {
        Node_StdInc(Node);
        assert(Node -> current -> elem[0] == '(');
        Node_StdInc(Node);
        res = NewTreeItem(ARCTG, 0, NULL, ExprLevel(Node));
        assert(Node -> current -> elem[0] == ')');
        Node_StdInc(Node);
      }
    else if ( stricmp(Node -> current -> elem, "ARCCTG") == 0 )
      {
        Node_StdInc(Node);
        assert(Node -> current -> elem[0] == '(');
        Node_StdInc(Node);
        res = NewTreeItem(ARCCTG, 0, NULL, ExprLevel(Node));
        assert(Node -> current -> elem[0] == ')');
        Node_StdInc(Node);
      }
    else if ( stricmp(Node -> current -> elem, "LN") == 0 )
      {
        Node_StdInc(Node);
        assert(Node -> current -> elem[0] == '(');
        Node_StdInc(Node);
        res = NewTreeItem(LN, 0, NULL, ExprLevel(Node));
        assert(Node -> current -> elem[0] == ')');
        Node_StdInc(Node);
      }
    else if ( stricmp(Node -> current -> elem, "SQR") == 0 )
      {
        Node_StdInc(Node);
        assert(Node -> current -> elem[0] == '(');
        Node_StdInc(Node);
        res = NewTreeItem(SQR, 0, NULL, ExprLevel(Node));
        assert(Node -> current -> elem[0] == ')');
        Node_StdInc(Node);
      }
    else if ( stricmp(Node -> current -> elem, "SQRT") == 0 )
      {
        Node_StdInc(Node);
        assert(Node -> current -> elem[0] == '(');
        Node_StdInc(Node);
        res = NewTreeItem(SQRT, 0, NULL, ExprLevel(Node));
        assert(Node -> current -> elem[0] == ')');
        Node_StdInc(Node);
      }
    else assert(0);
    return res;
  }


tree_item_diff_t* QuarkLevel(node_t(string)* Node)
  {
    if ( Node -> current == NULL ) return NULL;

    tree_item_diff_t* res = ( tree_item_diff_t* ) calloc(1, sizeof(*res));
    char* EndPtr = NULL;
    res = Tree_CreateItem(NUMBER, strtod(Node -> current -> elem, &EndPtr));
    return res;
  }



int Tree_GetExpression(tree_diff_t* Tree, char* input_file_name)
  {
    assert(Tree);
    assert(input_file_name);

    FILE* input_file = fopen(input_file_name, "r");
    assert(input_file);

    char* str = ( char* ) calloc(MAX_EXPRESSION_STRING + 1, sizeof(*str));

    node_t(string) Node;
    Node_ctor(&Node);

    while ( 1 == fscanf(input_file, "%10s", str) ) Node_TailPush(&Node, str);
    Node.current = Node.head;
    Tree -> root = ExprLevel(&Node);

    Node_dtor(&Node);

    free(str);
    str = NULL;

    return 0;
  }



 void ListTreeElems( tree_item_diff_t* Item, FILE* output_file)
   {
     if ( Item != NULL )
       {
         if ( Item -> item_type == NUMBER) fprintf(output_file, "%lg ", Item -> number);
         else if ( Item -> item_type == VARIABLE) fprintf(output_file, "x ");
         #define DEF_ITEM_TYPE(type, num, sign,diff_code, simp_code, tex_dump_code) \
         else if ( Item -> item_type == type )       \
           {                                              \
             fprintf(output_file, "( ");                    \
             ListTreeElems(Item -> left, output_file);    \
             fprintf(output_file,"%s ", #sign);           \
             ListTreeElems(Item -> right, output_file);   \
             fprintf(output_file, ") ");                  \
           }
         #include "C:\Users\Äåíèñ\Desktop\Projects\C Codeblocks Projects\DiffTree\Tree_item_diff_types.h"
         #undef DEF_ITEM_TYPE
       }
   }



int Tree_OutputExpression(tree_diff_t* Tree, char* output_file_name)
  {
    assert(Tree);
    assert(output_file_name);
    FILE* output_file = fopen(output_file_name, "w");
    assert(output_file);
    ListTreeElems(Tree -> root, output_file);
    fclose(output_file);
  }



int Tree_CreateRoot(tree_diff_t* Tree, tree_item_diff_type item_type, double number)
  {
    assert(Tree);
    Tree -> root = Tree_CreateItem(item_type, number);
    return 0;
  }


int Tree_LeftPush(tree_diff_t* Tree, tree_item_diff_t* This, tree_item_diff_type item_type, double number)
  {
    assert(Tree);
    assert(This);
    This -> left = Tree_CreateItem(item_type, number);
    return 0;
  }


int Tree_RightPush(tree_diff_t* Tree, tree_item_diff_t* This, tree_item_diff_type item_type, double number)
  {
    assert(Tree);
    assert(This);
    This -> right = Tree_CreateItem(item_type, number);
    return 0;
  }



int Tree_LeftPushItem(tree_diff_t* Tree, tree_item_diff_t* This, tree_item_diff_t* Item)
  {
    assert(Tree);
    assert(This);
    assert(Item);
    This -> left = Item;
    return 0;
  }



int Tree_RightPushItem(tree_diff_t* Tree, tree_item_diff_t* This, tree_item_diff_t* Item)
  {
    assert(Tree);
    assert(This);
    assert(Item);
    This -> right = Item;
    return 0;
  }



void tree_dt(tree_item_diff_t* tree_item)
  {
    if ( tree_item != NULL )
      {
        tree_dt(tree_item -> left );
        tree_dt(tree_item -> right);
        free(tree_item);
      }
  }


int Tree_sub_dtor(tree_diff_t* Tree, tree_item_diff_t* tree_item)
  {
    assert(Tree);
    assert(tree_item);
    if ( tree_item == Tree -> root ) Tree -> root = NULL;
    tree_dt(tree_item);
    return 0;
  }

int Tree_Branch_dtor(tree_item_diff_t* tree_item)
  {
    assert(tree_item);
    tree_dt(tree_item);
    return 0;
  }


int Tree_dtor(tree_diff_t* Tree)
  {
    assert(Tree);
    Tree -> tree_left_canary = Tree -> tree_right_canary = 0;

    if ( Tree -> root != NULL ) tree_dt(Tree -> root);
    Tree -> root = NULL;

    return 0;
  }


void Tree_MainCopyItems(tree_item_diff_t* old_item, tree_item_diff_t* new_item)
  {
    if ( old_item != NULL )
      {
        new_item -> left  = ( old_item -> left  != NULL ) ? Tree_CreateItem(old_item -> left -> item_type, old_item -> left -> number) : NULL;
        new_item -> right = ( old_item -> right != NULL ) ? Tree_CreateItem(old_item -> right -> item_type, old_item -> right -> number) : NULL;
        Tree_MainCopyItems(old_item -> left, new_item -> left);
        Tree_MainCopyItems(old_item -> right, new_item -> right);
      }
  }



tree_item_diff_t* Tree_CopyItems(tree_item_diff_t* This)
  {
    assert(This);
    tree_item_diff_t* new_item = Tree_CreateItem(This -> item_type, This -> number);
    Tree_MainCopyItems(This, new_item);
    return new_item;
  }




int Tree_CopyTree(tree_diff_t* Tree1, tree_diff_t* Tree2)
  {
    assert(Tree1);
    assert(Tree2);
    Tree2 -> root = Tree_CopyItems(Tree1 -> root);
    return 0;
  }




tree_item_diff_t* Tree_MainDiff(tree_item_diff_t* Item)
  {
    #define LEFT Item -> left
    #define RIGHT Item -> right
    #define DIFF(var) Tree_MainDiff(var)
    #define COPY(var) Tree_CopyItems(var)
    if ( Item != NULL )
      {
        tree_item_diff_t* new_item = NULL;
        switch( Item -> item_type)
          {
            case NUMBER:
              {
                new_item = Tree_CreateItem(NUMBER, 0);
              }
              break;
            case VARIABLE:
              {
                new_item = Tree_CreateItem(NUMBER, 1);
              }
              break;
            #define DEF_ITEM_TYPE(type, num, sign, diff_code, simp_code, tex_dump_code) \
              case type: \
                diff_code \
                break;
            #include "C:\Users\Äåíèñ\Desktop\Projects\C Codeblocks Projects\DiffTree\Tree_item_diff_types.h"
            #undef DEF_ITEM_TYPE
            default: assert(0);
          }
        return new_item;
      }
    else return NULL;
    #undef LEFT
    #undef RIGHT
    #undef DIFF
    #undef COPY
  }



int Tree_Diff(tree_diff_t* Tree, tree_diff_t* Diff_Tree)
  {
    assert(Tree);
    assert(Diff_Tree);

    Tree_dtor(Diff_Tree);
    Tree_ctor(Diff_Tree);

    Diff_Tree -> root = Tree_MainDiff(Tree -> root);

    return 0;
  }




void Tree_MainTexDump(tree_item_diff_t* Item, char* output_buf, unsigned int* output_pos)
  {
    if ( Item != NULL )
      {
        switch ( Item -> item_type)
          {
            #define DEF_ITEM_TYPE(type, num, sign, diff_code, simp_code, tex_dump_code) \
              case type:      \
                tex_dump_code \
                break;
            #include "C:\Users\Äåíèñ\Desktop\Projects\C Codeblocks Projects\DiffTree\Tree_item_diff_types.h"
              case NUMBER:
                if ( Item -> number < 0 ) *output_pos += sprintf(output_buf + *output_pos, "\\left( ");
                *output_pos += sprintf(output_buf + *output_pos, "%lg ", Item -> number);
                if ( Item -> number < 0 ) *output_pos += sprintf(output_buf + *output_pos, "\\right) ");
                break;
              case VARIABLE:
                *output_pos += sprintf(output_buf + *output_pos, "x ");
                break;
              default: assert(0);
            #undef DEF_ITEM_TYPE
          }
      }
  }




char* Tree_TexDump(tree_diff_t* Tree)
  {
    assert(Tree);
    char* output_buf = ( char* ) calloc(MAX_TEX_DUMP_SIZE, sizeof(*output_buf));
    unsigned int output_pos = 0;
    Tree_MainTexDump(Tree -> root, output_buf, &output_pos);
    return output_buf;
  }

tree_item_diff_t* Tree_Simplify(tree_item_diff_t* Item, bool* simpl_flag)
  {
    #define LEFT Item -> left
    #define RIGHT Item -> right
    #define L_NUM Item -> left -> number
    #define R_NUM Item -> right -> number
    #define DESTRUCT(Elem) Tree_Branch_dtor(Elem)
    if ( Item != NULL )
      {
        tree_item_diff_t* ptr = NULL;
        switch( Item -> item_type)
          {
            #define DEF_ITEM_TYPE(type, num, sign, diff_code, simp_code, tex_dump_code) \
              case type:   \
                simp_code; \
                break;
            #include "C:\Users\Äåíèñ\Desktop\Projects\C Codeblocks Projects\DiffTree\Tree_item_diff_types.h"
            #undef DEF_ITEM_TYPE;
            default: break;
          }
        Item -> left = Tree_Simplify(Item -> left, simpl_flag);
        Item -> right = Tree_Simplify(Item -> right, simpl_flag);
        return Item;
      }
    else return NULL;
    #undef LEFT
    #undef RIGHT
    #undef L_NUM
    #undef R_NUM
    #undef DESTRUCT
  }




int Tree_TexSimplifyExpression(tree_diff_t* Initial_Tree, tree_diff_t* Tree, char* output_file_name)
  {
    assert(Tree);
    assert(output_file_name);

    FILE* output_file = fopen(output_file_name, "w");
    assert(output_file);

    unsigned int text_elem = rand() % MAX_TEXT_ARRAY_SIZE;
    char* tex_dump_buf = NULL;
    bool simpl_flag = false;

    fprintf(output_file, "\\documentclass[12pt]{book}\n\\usepackage[cp1251]{inputenc}\n\\usepackage[russian]{babel}\n\\begin{document}\n");

    tex_dump_buf = Tree_TexDump(Initial_Tree);
    fprintf(output_file, "\\begin{center}\n \\section*{\\huge Derivative 2016 } \n\\end{center}\n\nWe can see that\n");
    fprintf(output_file, "\\[\nf(x) = %s\n\\]\n", tex_dump_buf);
    free(tex_dump_buf);
    tex_dump_buf = NULL;

    fprintf(output_file, "\nIt's obvious that\n");
    tex_dump_buf = Tree_TexDump(Tree);
    fprintf(output_file, "\\[\nf'(x) = %s\n\\]\n", tex_dump_buf);

    free(tex_dump_buf);
    tex_dump_buf = NULL;

    Tree -> root = Tree_Simplify(Tree -> root, &simpl_flag);

    fprintf(output_file, "%s", text_array[text_elem]);
    text_elem = ( text_elem + 1 ) % MAX_TEXT_ARRAY_SIZE;
    tex_dump_buf = Tree_TexDump(Tree);
    fprintf(output_file, "\\[\nf'(x) = %s\n\\]\n", tex_dump_buf);
    free(tex_dump_buf); tex_dump_buf = NULL;

    while ( simpl_flag )
      {
        simpl_flag = false;
        Tree -> root = Tree_Simplify(Tree -> root, &simpl_flag);
        if ( simpl_flag )
          {
            fprintf(output_file, "%s", text_array[text_elem]);
            text_elem = ( text_elem + 1 ) % MAX_TEXT_ARRAY_SIZE;
            tex_dump_buf = Tree_TexDump(Tree);
            fprintf(output_file, "\\[\nf'(x) = %s\n\\]\n", tex_dump_buf);
            free(tex_dump_buf); tex_dump_buf = NULL;
          }
      }

    fprintf(output_file, "\\end{document}\n");

    free(tex_dump_buf); tex_dump_buf = NULL;
    fclose(output_file); output_file = NULL;
    return 0;
  }




int main()
  {
    tree_t(diff) Tree;
    tree_t(diff) Tree2;
    Tree_ctor(&Tree); Tree_ctor(&Tree2);
    Tree_GetExpression(&Tree, "input.txt");
    Tree_Diff(&Tree, &Tree2);
    Tree_TexSimplifyExpression(&Tree, &Tree2, "output.tex");
    Tree_OutputExpression(&Tree2, "output.txt");
    Tree_dtor(&Tree);
    return 0;
  }
