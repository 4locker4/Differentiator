#include "../inc/Differentiator.h"

NODE * Taylor (TREE * tree)
{
    my_assert (tree);
    
    if (!tree->status)
    {
        printf ("Please, firstly init the expression\n");

        return NULL;
    }

    int max_degree = 0;

    printf ("Enter the maximum degree to which you want to expand: ");

    while (scanf ("%d", &max_degree) != 1 || max_degree < 0)
    {
        printf ("Please, enter only positive numbers\n");

        BufferCleaner ();
    }

    printf ("\n");

    printf ("Please, enter the variable by which to differentiate: ");

    BufferCleaner ();

    char var_name = NULL;

    while (scanf ("%c", &var_name) != 1)
    {
        printf ("Please, enter the variable\n");

        BufferCleaner ();
    }

    printf ("\n");

    BufferCleaner ();

    printf ("In what point do You want to have Taylor series: ");

    int x0 = 0;

    while (scanf ("%d", &x0) != 1)
    {
        printf ("Please, enter the num\n");

        BufferCleaner ();
    }

    printf ("\n");

    tree->taylor_series = TaylorRecursy (tree->root, var_name, &tree->depth, max_degree, x0);

    return NULL;
}

NODE * TaylorRecursy (NODE * node, char var_name, int * depth, int max_depth, int x0)
{
    my_assert (node);
    my_assert (depth);

    if (*depth <= max_depth)
    {
        NODE * dif = Diffirentiation (node, var_name);

        NODE * derivative = NUM_ (Eval (node, x0));

        node = DIV_ (MUL_ (derivative, POW_ (SUB_ (VAR_ (var_name), NUM_ (x0)), NUM_ (*depth))), NUM_ (Factorial (*depth)));

        *depth += 1;

        node = ADD_ (node, TaylorRecursy (dif, var_name, depth, max_depth, x0));

        *depth -= 1;

        return node;
    }
    else
        return VAR_ ('o');
}

NODE * TakeHigherOrderDerivative (NODE * node, char var_name, int * depth, int max_degree)
{
    my_assert (node);
    my_assert (depth);

    *depth += 1;

    if (*depth <= max_degree)
        node = TakeHigherOrderDerivative (node, var_name, depth, max_degree);
    else
        return NULL;

    node = Diffirentiation (node, var_name);
    
    *depth -= 1;

    return NULL;
}