#include "../inc/Differentiator.h"

const char * FILE_WITH_DATA = "./inc/ExpressionData.txt";

// Из файла в строку затем удалить все пробелы и это expr
// 

TREE * TreeCtor ()
{
    TREE * tree = (TREE *) calloc (1, sizeof (TREE));
    my_assert (tree);

    tree->data_base_file = FILE_WITH_DATA;
    tree->status = true;
    
    tree->expr  = FileToStr (tree->data_base_file, &tree->tree_size);
    tree->root  = GetG (tree->expr, &tree->text_pointer);
    tree->depth = 0;

    tree->diffed_expr_root = tree->root;

    StartDiff (tree);

    return tree;
}

NODE * GetG (char * expr, int * pointer)
{
    my_assert (expr);
    my_assert (pointer);

    NODE * val = GetExpression (expr, pointer);

    if (expr[*pointer] != '$')
        SyntaxError (expr, pointer);

    *pointer += 1;

    return val;
}

NODE * GetExpression (char * expr, int * pointer)
{
    my_assert (expr);
    my_assert (pointer);

    NODE * val1 = GetT (expr, pointer);

    while (expr[*pointer] == '+' || expr[*pointer] == '-')
    {
        if (isspace (expr[*pointer]))
        {
            *pointer += 1;
            continue;
        }

        int op = expr[*pointer];

        *pointer += 1;

        NODE * val2 = GetT (expr, pointer);

        if (op == '+')
            val1 = NewOpNode (OP, ADD, val1, val2);
        else
            val1 = NewOpNode (OP, SUB, val1, val2);
    }

    return val1;
}

NODE * GetT (char * expr, int * pointer)
{
    my_assert (expr);
    my_assert (pointer);

    NODE * val1 = GetDegree (expr, pointer);

    while (expr[*pointer] == '*' || expr[*pointer] == '/')
    {
        int op = expr[*pointer];

        *pointer += 1;

        NODE * val2 = GetDegree (expr, pointer);

        if (op == '*')
            val1 = NewOpNode (OP, MUL, val1, val2);
        else
            val1 = NewOpNode (OP, DIV, val1, val2);
    }

    return val1;
}

NODE * GetDegree (char * expr, int * pointer)
{
    my_assert (expr);
    my_assert (pointer);

    NODE * val1 = GetP (expr, pointer);

    while (expr[*pointer] == '^')
    {
        *pointer += 1;

        NODE * val2 = GetP (expr, pointer);

        val1 = NewOpNode (OP, POW, val1, val2);
    }

    return val1;
}

NODE * GetP (char * expr, int * pointer)
{
    my_assert (expr);
    my_assert (pointer);

    if (expr[*pointer] == '(')
    {
        *pointer += 1;

        NODE * val = GetExpression (expr, pointer);

        if (expr[*pointer] != ')')
            SyntaxError (expr, pointer);

        *pointer += 1;
        
        return val;
    }
    else
        return GetFunc (expr, pointer);
}

NODE * GetFunc (char * expr, int * pointer)
{
    my_assert (expr);
    my_assert (pointer);

    switch (expr[*pointer])
    {
        case SIN:
        {
            *pointer += 1;

            if (expr[*pointer] != '(')
                return NewVarNode (OP, expr[*pointer - 1], NULL, NULL);
            
            *pointer += 1;

            NODE* op = NewOpNode (OP, SIN, GetExpression (expr, pointer), NULL);

            if (expr[*pointer] != ')')
                SyntaxError (expr, pointer);
            
            *pointer += 1;

            return op;
        }
        case COS:
        {
            *pointer += 1;

            if (expr[*pointer] != '(')
                return NewVarNode (OP, expr[*pointer - 1], NULL, NULL);

            NODE* op = NewOpNode (OP, COS, GetExpression (expr, pointer), NULL);

            if (expr[*pointer] != ')')
                SyntaxError (expr, pointer);
            
            *pointer += 1;

            return op;
        }
        case LN:
        {
            *pointer += 1;

            if (expr[*pointer] != '(')
                return NewVarNode (OP, expr[*pointer - 1], NULL, NULL);

            NODE* op = NewOpNode (OP, LN, GetExpression (expr, pointer), NULL);

            if (expr[*pointer] != ')')
                SyntaxError (expr, pointer);
            
            *pointer += 1;

            return op;
        }
        case SQRT:
        {
            *pointer += 1;

            if (expr[*pointer] != '(')
                return NewVarNode (OP, expr[*pointer - 1], NULL, NULL);

            NODE* op = NewOpNode (OP, SQRT, GetExpression (expr, pointer), NULL);
            
            if (expr[*pointer] != ')')
                SyntaxError (expr, pointer);

            *pointer += 1;

            return op;
        }
        default:
            return GetN (expr, pointer);
    }
}

NODE * GetN (char * expr, int * pointer)
{
    my_assert (expr);
    my_assert (pointer);

    int val = 0;

    int old_pointer = *pointer;

    if (expr[*pointer] >= 'a' && expr[*pointer] <= 'z')
    {
        NODE * var = NewVarNode (VAR, expr[*pointer], NULL, NULL);

        *pointer += 1;

        return var;
    }  

    while ('0' <= expr[*pointer] && expr[*pointer] <= '9')
    {
        val = val * 10 + expr[*pointer] - '0';
        
        *pointer += 1;
    }

    if (*pointer == old_pointer)
        SyntaxError (expr, pointer);    
    
    return NewNumNode (NUM, val, NULL, NULL);
}

int SyntaxError (char * expr, int * pointer)
{
    printf ("%c\n", expr[*pointer]);
    printf ("ERROR\n");
    exit (1);
}

NODE * NewNumNode (TYPES type, int elem, NODE * left_node, NODE * right_node)
{
    NODE * node = (NODE *) calloc (1, sizeof (NODE));
    my_assert (node);

    node->node_type = type;

    if (type == NUM) node->data.val = elem;

    node->left  = left_node;
    node->right = right_node;

    return node;
}

NODE * NewVarNode (TYPES type, char var, NODE * left_node, NODE * right_node)
{
    NODE * node = (NODE *) calloc (1, sizeof (NODE));
    my_assert (node);

    node->node_type = type;

    if (type == VAR) node->data.var.var_name = var;

    node->left  = left_node;
    node->right = right_node;

    return node;
}

NODE * NewOpNode (TYPES type, OPERATORS op, NODE * left_node, NODE * right_node)
{
    NODE * node = (NODE *) calloc (1, sizeof (NODE));
    my_assert (node);

    node->node_type = type;

    if (type == OP) node->data.op = op;

    node->left  = left_node;
    node->right = right_node;

    return node;
}

int TreeDtor (NODE * node, WHAT_DTOR branch)
{
    my_assert (node);

    if (branch == ALL_NODES)
    {
        RecurcyDtor (node->left);
        RecurcyDtor (node->right);
    }
    else if (branch == WITH_LEFT_BRACH)
        if (node->left)
            RecurcyDtor (node->left);
    else if (branch == WITH_RIGHT_BRANCH)
        if (node->right)
            RecurcyDtor (node->right);
    
    node->left  = NULL;
    node->right = NULL;

    node->data.op  = NOT_OPERATOR;
    node->data.val = 0;
    node->data.var.var_name = 0;
    node->data.var.var_val  = 0;

    node->node_type = NOT_INITED;

    node = NULL;
    free (node);
    
    return 0;
}

int RecurcyDtor (NODE * node)
{
    my_assert (node);

    if (node->left) RecurcyDtor (node->left);

    if (node->right) RecurcyDtor (node->right);

    node->left  = NULL;
    node->right = NULL;

    node->data.op  = NOT_OPERATOR;
    node->data.val = 0;
    node->data.var.var_name = 0;
    node->data.var.var_val  = 0;

    node->node_type = NOT_INITED;

    node = NULL;
    free (node);

    return 0;
}