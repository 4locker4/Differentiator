#include "../inc/Differentiator.h"

static char expr[100] = {};
static int  pointer = 0;

// Из файла в строку затем удалить все пробелы и это expr
// 

NODE * GetG ()
{
    scanf ("%s", &expr);

    NODE * val = GetExpression ();

    if (expr[pointer] != '$')
        SyntaxError ();

    pointer++;

    return val;
}

NODE * GetExpression ()
{
    NODE * val1 = GetT ();

    while (expr[pointer] == '+' || expr[pointer] == '-')
    {
        if (isspace (expr[pointer]))
        {
            pointer++;
            continue;
        }

        int op = expr[pointer];

        pointer++;

        NODE * val2 = GetT ();

        if (op == '+')
            val1 = NewOpNode (OP, ADD, val1, val2);
        else
            val1 = NewOpNode (OP, SUB, val1, val2);
    }

    return val1;
}

NODE * GetT ()
{
    NODE * val1 = GetDegree ();

    while (expr[pointer] == '*' || expr[pointer] == '/')
    {
        int op = expr[pointer];

        pointer++;

        NODE * val2 = GetDegree ();

        if (op == '*')
            val1 = NewOpNode (OP, MUL, val1, val2);
        else
            val1 = NewOpNode (OP, DIV, val1, val2);
    }

    return val1;
}

NODE * GetDegree ()
{
    NODE * val1 = GetP ();

    while (expr[pointer] == '^')
    {
        pointer++;

        NODE * val2 = GetP ();

        val1 = NewOpNode (OP, POW, val1, val2);
    }

    return val1;
}

NODE * GetP ()
{
    if (expr[pointer] == '(')
    {
        pointer++;

        NODE * val = GetExpression ();

        if (expr[pointer] != ')')
            SyntaxError ();

        pointer++;
        
        return val;
    }
    else
        return GetN ();
}

NODE * GetN ()
{
    int val = 0;

    int old_pointer = pointer;

    if (expr[pointer] >= 'a' && expr[pointer] <= 'z')       
        return NewVarNode (VAR, expr[pointer++], NULL, NULL);

    while ('0' <= expr[pointer] && expr[pointer] <= '9')
    {
        val = val * 10 + expr[pointer] - '0';
        
        pointer++;
    }

    if (pointer == old_pointer)
        SyntaxError ();
    
    return NewNumNode (NUM, val, NULL, NULL);
}

int SyntaxError ()
{
    printf ("%c\n", expr[pointer]);
    printf ("ERROR\n");
    exit (1);
}