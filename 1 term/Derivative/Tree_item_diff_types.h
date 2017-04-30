
DEF_ITEM_TYPE(ADD, 2, +,
{ new_item = NewTreeItem(ADD, 0, DIFF(LEFT), DIFF(RIGHT)); },
{
  if ( LEFT -> item_type == NUMBER && RIGHT -> item_type == NUMBER)
    {
      ptr = Tree_CreateItem(NUMBER, L_NUM + R_NUM );
      DESTRUCT(Item);
      Item = ptr;
      *simpl_flag = true;
    }
  else if ( LEFT -> item_type == NUMBER && L_NUM == 0 )
    {
      ptr = RIGHT;
      RIGHT = NULL;
      DESTRUCT(Item);
      Item = ptr;
      *simpl_flag = true;
    }
  else if ( RIGHT -> item_type == NUMBER && R_NUM == 0 )
    {
      ptr = LEFT;
      LEFT = NULL;
      DESTRUCT(Item);
      Item = ptr;
      *simpl_flag = true;
    }
},
{
  Tree_MainTexDump(Item -> left, output_buf, output_pos);
  *output_pos += sprintf(output_buf + *output_pos, "+ ");
  Tree_MainTexDump(Item -> right, output_buf, output_pos);
})

DEF_ITEM_TYPE(MUL, 3, *,
{ new_item = NewTreeItem(ADD, 0, NewTreeItem(MUL, 0, DIFF(LEFT), COPY(RIGHT)), NewTreeItem(MUL, 0, DIFF(RIGHT), COPY(LEFT))); },
{
  if ( RIGHT -> item_type == NUMBER && LEFT -> item_type == NUMBER)
    {
      ptr = Tree_CreateItem(NUMBER, L_NUM * R_NUM);
      DESTRUCT(Item);
      Item = ptr;
      *simpl_flag = true;
    }
  else if ( ( LEFT -> item_type == NUMBER && L_NUM == 0 ) || ( RIGHT -> item_type == NUMBER && R_NUM == 0 ) )
    {
      DESTRUCT(Item);
      Item = Tree_CreateItem(NUMBER, 0);
      *simpl_flag = true;
    }
  else if ( L_NUM == 1)
    {
      ptr = RIGHT;
      RIGHT = NULL;
      DESTRUCT(Item);
      Item = ptr;
      *simpl_flag = true;
    }
  else if ( R_NUM == 1)
    {
      ptr = LEFT;
      LEFT = NULL;
      DESTRUCT(Item);
      Item = ptr;
      *simpl_flag = true;
    }
},
{
  if ( Item -> left -> item_type == SUB || Item -> left -> item_type == ADD )
    {
      *output_pos += sprintf(output_buf + *output_pos, "\\left( ");
      Tree_MainTexDump(Item -> left, output_buf, output_pos);
      *output_pos += sprintf(output_buf + *output_pos, "\\right) ");
    }
  else Tree_MainTexDump(Item -> left, output_buf, output_pos);
  *output_pos += sprintf(output_buf + *output_pos, "* ");
  if ( Item -> right -> item_type == SUB || Item -> right -> item_type == ADD )
    {
      *output_pos += sprintf(output_buf + *output_pos, "\\left( ");
      Tree_MainTexDump(Item -> right, output_buf, output_pos);
      *output_pos += sprintf(output_buf + *output_pos, "\\right) ");
    }
  else Tree_MainTexDump(Item -> right, output_buf, output_pos);
})




DEF_ITEM_TYPE(SUB, 4, -,
{ new_item = NewTreeItem(SUB, 0, DIFF(LEFT), DIFF(RIGHT)); },
{
  if ( LEFT -> item_type == NUMBER && RIGHT -> item_type == NUMBER)
    {
      ptr = Tree_CreateItem(NUMBER, L_NUM - R_NUM );
      DESTRUCT(Item);
      Item = ptr;
      *simpl_flag = true;
    }
  else if ( LEFT -> item_type == NUMBER && L_NUM == 0 )
    {
      ptr = RIGHT;
      RIGHT = NULL;
      DESTRUCT(Item);
      Item = ptr;
      *simpl_flag = true;
    }
  else if ( RIGHT -> item_type == NUMBER && R_NUM == 0 )
    {
      ptr = LEFT;
      LEFT = NULL;
      DESTRUCT(Item);
      Item = ptr;
      *simpl_flag = true;
    }
},
{
  Tree_MainTexDump(Item -> left, output_buf, output_pos);
  *output_pos += sprintf(output_buf + *output_pos, "- ");
  Tree_MainTexDump(Item -> right, output_buf, output_pos);
})





DEF_ITEM_TYPE(DIV, 5, /,
{ new_item = NewTreeItem(DIV, 0, NewTreeItem(SUB, 0, NewTreeItem(MUL, 0, DIFF(LEFT), COPY(RIGHT)), NewTreeItem(MUL, 0, DIFF(RIGHT), COPY(LEFT) ) ), NewTreeItem(SQR, 0, NULL, COPY(RIGHT) ) ); },
{
  if ( LEFT -> item_type == NUMBER && RIGHT -> item_type == NUMBER && R_NUM != 0 )
    {
      ptr = Tree_CreateItem(NUMBER, L_NUM / R_NUM );
      DESTRUCT(Item);
      Item = ptr;
      *simpl_flag = true;
    }
  else if ( LEFT -> item_type == NUMBER && L_NUM == 0 )
    {
      DESTRUCT(Item);
      Item = Tree_CreateItem(NUMBER, 0);
      *simpl_flag = true;
    }
  else if ( R_NUM == 1 )
    {
      ptr = LEFT;
      LEFT = NULL;
      DESTRUCT(Item);
      Item = ptr;
      *simpl_flag = true;
    }
},
{
  *output_pos += sprintf(output_buf + *output_pos, "\\frac{");
  Tree_MainTexDump(Item -> left, output_buf, output_pos);
  *output_pos += sprintf(output_buf + *output_pos,"}{");
  Tree_MainTexDump(Item -> right, output_buf, output_pos);
  *output_pos += sprintf(output_buf + *output_pos, "}");
})




DEF_ITEM_TYPE(POW, 6, ^,
{ new_item = NewTreeItem(MUL, 0, COPY(Item), NewTreeItem(ADD, 0, NewTreeItem(MUL, 0, DIFF(RIGHT), NewTreeItem(LN, 0, NULL, COPY(LEFT)) ), NewTreeItem(MUL, 0, COPY(RIGHT), NewTreeItem(DIV, 0, DIFF(LEFT), COPY(LEFT))))); },
{
  if ( ( LEFT -> item_type != NUMBER || L_NUM != 0 ) && (RIGHT -> item_type == NUMBER && R_NUM == 0)  )
    {
      DESTRUCT(Item);
      Item = Tree_CreateItem(NUMBER, 1);
      *simpl_flag = true;
    }
  else if ( RIGHT -> item_type == NUMBER && R_NUM == 1)
    {
      ptr = LEFT;
      LEFT = NULL;
      DESTRUCT(Item);
      Item = ptr;
      *simpl_flag = true;
    }
},
{
  *output_pos += sprintf(output_buf + *output_pos, "{");
  if ( Item -> left -> item_type == NUMBER || Item -> left -> item_type == VARIABLE )
    Tree_MainTexDump(Item -> left, output_buf, output_pos);
  else
    {
      *output_pos += sprintf(output_buf + *output_pos, "\\left( ");
      Tree_MainTexDump(Item -> left, output_buf, output_pos);
      *output_pos += sprintf(output_buf + *output_pos, "\\right) ");
    }
  *output_pos += sprintf(output_buf + *output_pos, "}");
  *output_pos += sprintf(output_buf + *output_pos, "^{");
  if ( Item -> right -> item_type == NUMBER || Item -> right -> item_type == VARIABLE)
    Tree_MainTexDump(Item -> right, output_buf, output_pos);
  else
    {
      *output_pos += sprintf(output_buf + *output_pos, "\\left( ");
      Tree_MainTexDump(Item -> right, output_buf, output_pos);
      *output_pos += sprintf(output_buf + *output_pos, "\\right) ");
    }
  *output_pos += sprintf(output_buf + *output_pos, "}");
}  )





DEF_ITEM_TYPE(SIN, 7, sin,
{ new_item = NewTreeItem(MUL, 0, NewTreeItem(COS, 0, NULL, COPY(RIGHT)), DIFF(RIGHT)); },
{
  if ( RIGHT -> item_type == NUMBER && R_NUM == 0 )
    {
      DESTRUCT(Item);
      Item = Tree_CreateItem(NUMBER, 0);
      *simpl_flag = true;
    }
},
{
  *output_pos += sprintf(output_buf +*output_pos, "\\sin \\left( ");
  Tree_MainTexDump(Item -> right, output_buf, output_pos);
  *output_pos += sprintf(output_buf + *output_pos, "\\right) ");
} )




DEF_ITEM_TYPE(COS, 8, cos,
{ new_item = NewTreeItem(MUL, 0, NewTreeItem(MUL, 0, Tree_CreateItem(NUMBER, -1), NewTreeItem(SIN, 0, NULL, COPY(RIGHT))), DIFF(RIGHT)); },
{
  if ( RIGHT -> item_type == NUMBER && R_NUM == 0 )
    {
      DESTRUCT(Item);
      Item = Tree_CreateItem(NUMBER, 1);
      *simpl_flag = true;
    }
},
{
  *output_pos += sprintf(output_buf +*output_pos, "\\cos \\left( ");
  Tree_MainTexDump(Item -> right, output_buf, output_pos);
  *output_pos += sprintf(output_buf + *output_pos, "\\right) ");
})




DEF_ITEM_TYPE(TG, 9, tg,
{ new_item = NewTreeItem(DIV, 0, DIFF(RIGHT), NewTreeItem(SQR, 0, NULL, NewTreeItem(COS, 0, NULL, COPY(RIGHT)))); },
{
  if ( RIGHT -> item_type == NUMBER && R_NUM == 0 )
    {
      DESTRUCT(Item);
      Item = Tree_CreateItem(NUMBER, 0);
      *simpl_flag = true;
    }
},
{
  *output_pos += sprintf(output_buf +*output_pos, "\\tg \\left( ");
  Tree_MainTexDump(Item -> right, output_buf, output_pos);
  *output_pos += sprintf(output_buf + *output_pos, "\\right) ");
})



DEF_ITEM_TYPE(CTG, 10, ctg,
{ new_item = NewTreeItem(MUL, 0, Tree_CreateItem(NUMBER, -1), NewTreeItem(DIV, 0, DIFF(RIGHT), NewTreeItem(SQR, 0, NULL, NewTreeItem(SIN, 0, NULL, COPY(RIGHT))))); },
{ },
{
  *output_pos += sprintf(output_buf +*output_pos, "\\ctg \\left( ");
  Tree_MainTexDump(Item -> right, output_buf, output_pos);
  *output_pos += sprintf(output_buf + *output_pos, "\\right) ");
})



DEF_ITEM_TYPE(ARCSIN, 11, arcsin,
{ new_item = NewTreeItem(DIV, 0, DIFF(RIGHT), NewTreeItem(SQRT, 0, NULL, NewTreeItem(SUB, 0, Tree_CreateItem(NUMBER, 1), NewTreeItem(SQR, 0, NULL, COPY(RIGHT))))); },
{ },
{
  *output_pos += sprintf(output_buf +*output_pos, "\\arcsin \\left( ");
  Tree_MainTexDump(Item -> right, output_buf, output_pos);
  *output_pos += sprintf(output_buf + *output_pos, "\\right) ");
})



DEF_ITEM_TYPE(ARCCOS, 12, arccos,
{ new_item = NewTreeItem(MUL, 0, Tree_CreateItem(NUMBER, -1), NewTreeItem(DIV, 0, DIFF(RIGHT), NewTreeItem(SQRT, 0, NULL, NewTreeItem(SUB, 0, Tree_CreateItem(NUMBER, 1), NewTreeItem(SQR, 0, NULL, COPY(RIGHT)))))); },
{ },
{
  *output_pos += sprintf(output_buf +*output_pos, "\\arccos \\left( ");
  Tree_MainTexDump(Item -> right, output_buf, output_pos);
  *output_pos += sprintf(output_buf + *output_pos, "\\right) ");
})



DEF_ITEM_TYPE(ARCTG, 13, arctg,
{ new_item = NewTreeItem(DIV, 0, DIFF(RIGHT), NewTreeItem(ADD, 0, Tree_CreateItem(NUMBER, 1), NewTreeItem(SQR, 0, NULL, COPY(RIGHT)))); },
{ },
{
  *output_pos += sprintf(output_buf +*output_pos, "\\arctg \\left( ");
  Tree_MainTexDump(Item -> right, output_buf, output_pos);
  *output_pos += sprintf(output_buf + *output_pos, "\\right) ");
})



DEF_ITEM_TYPE(ARCCTG, 14, arcctg,
{ new_item = NewTreeItem(MUL, 0, Tree_CreateItem(NUMBER, -1), NewTreeItem(DIV, 0, DIFF(RIGHT), NewTreeItem(ADD, 0, Tree_CreateItem(NUMBER, 1), NewTreeItem(SQR, 0, NULL, COPY(RIGHT))))); },
{ },
{
  *output_pos += sprintf(output_buf +*output_pos, "\\arcctg \\left( ");
  Tree_MainTexDump(Item -> right, output_buf, output_pos);
  *output_pos += sprintf(output_buf + *output_pos, "\\right) ");
} )



DEF_ITEM_TYPE(LN, 15, ln,
{ new_item = NewTreeItem(DIV, 0, DIFF(RIGHT), COPY(RIGHT)); },
{
  if ( RIGHT -> item_type == NUMBER && R_NUM == 1 )
    {
      DESTRUCT(Item);
      Item = Tree_CreateItem(NUMBER, 0);
      *simpl_flag = true;
    }
},
{
  *output_pos += sprintf(output_buf +*output_pos, "\\ln \\left( ");
  Tree_MainTexDump(Item -> right, output_buf, output_pos);
  *output_pos += sprintf(output_buf + *output_pos, "\\right) ");
})




DEF_ITEM_TYPE(SQR, 16, sqr,
{ new_item = NewTreeItem(MUL, 0, NewTreeItem(MUL, 0, Tree_CreateItem(NUMBER, 2), COPY(RIGHT)), DIFF(RIGHT)); },
{
  if ( RIGHT -> item_type == NUMBER)
    {
      ptr = Tree_CreateItem(NUMBER, R_NUM * L_NUM);
      DESTRUCT(Item);
      Item = ptr;
      *simpl_flag = true;
    }
},
{
  if ( Item -> right -> item_type == VARIABLE || Item -> right -> item_type == NUMBER)
    Tree_MainTexDump(Item -> right, output_buf, output_pos);
  else
    {
      *output_pos += sprintf(output_buf +*output_pos, "\\left( ");
      Tree_MainTexDump(Item -> right, output_buf, output_pos);
      *output_pos += sprintf(output_buf + *output_pos, "\\right) ");
    }
  *output_pos += sprintf(output_buf + *output_pos, "^2 ");
})




DEF_ITEM_TYPE(SQRT, 17, sqrt,
{ new_item = NewTreeItem(DIV, 0, DIFF(RIGHT), NewTreeItem(MUL, 0, Tree_CreateItem(NUMBER, 2), COPY(Item))); },
{
  if ( RIGHT -> item_type == NUMBER && R_NUM >= 0 )
    {
      ptr = Tree_CreateItem(NUMBER, sqrt(R_NUM) );
      DESTRUCT(Item);
      Item = ptr;
      *simpl_flag = true;
    }
},
{
  *output_pos += sprintf(output_buf +*output_pos, "\\sqrt{");
  Tree_MainTexDump(Item -> right, output_buf, output_pos);
  *output_pos += sprintf(output_buf + *output_pos, "\\mathstrut } ");
})
