#include "../inc/Differentiator.h"

// Тейлор в тех
// Упрощалка

static nodeElem GLOBAL_X = 0;

const char * GOD_SAY_NO_COLOR   = "#FF7B61";
const char * GOD_SAY_YES_COLOR  = "#B2EC5D";

nodeElem Eval (NODE * node, int x0)
{
    my_assert (node);

    if (node->node_type == NUM) 
        return node->data.val;
    else if (node->node_type == VAR)
        return x0;
    else if (node->node_type = OP)
        switch (node->data.op)
        {
            case ADD:
                return Eval (node->left, x0) + Eval (node->right, x0);
            case SUB:
                return Eval (node->left, x0) - Eval (node->right, x0);
            case MUL:
                return Eval (node->left, x0) * Eval (node->right, x0);
            case DIV:
                return Eval (node->left, x0) / Eval (node->right, x0);
            case SIN:
                return sin (Eval (node->left, x0));
            case COS:
                return cos (Eval (node->left, x0));
            case LN:
                return log (Eval (node->left, x0));
            case POW:
            {
                double res = pow ((double) Eval (node->left, x0), (double) Eval (node->right, x0));

                return res;
            }
            case SQRT:
            {
                double res = pow (Eval (node->left, x0), 0.5);

                return res;
            }
            case NOT_INITED:
            {
                printf ("not inited node. Data: %d\n"
                        "node.left  = %p\n"
                        "node.right = %p\n", node->data.op, node->left, node->right);
                exit (-1);
            }
            default:
            {
                printf ("Error while calculating %d\n", node->data.op);
                exit (-1);
            }
        }
    else
    {
        printf ("Type is not var/op/num\n");
        exit (-1);
    }
    
    return 0;
}

NODE * StartDiff (TREE * tree)
{
    my_assert (tree);

    if (tree->status == NOT_INITED)
    {
        printf ("Please, firstly create a expression\n");

        return NULL;
    }

    printf ("Please, enter the variable by which to differentiate: ");

    char var_name = NULL;

    while (scanf ("%c", &var_name) != 1)
    {
        printf ("Please, enter the variable\n");

        BufferCleaner ();
    }

    printf ("\n");

    printf ("Order to take the derivative: ");

    int order = -1;

    while ((scanf ("%d", &order) != 1) || order < 0)
    {
        printf ("Please, enter only positive numbers\n");

        BufferCleaner ();
    }

    printf ("\n");

    //TakeHigherOrderDerivative (tree, var_name, order);

    return NULL;
}

NODE * Diffirentiation (NODE * dif, char var)
{
    my_assert (dif);

    switch (dif->node_type)
    {
        case NUM:
            return NewNumNode (NUM, 0, NULL, NULL);
        case VAR:
        {
            if (dif->data.var.var_name == var)
                return NewNumNode (NUM, 1, NULL, NULL);
            else
                return NewNumNode (NUM, 0, NULL, NULL);            
        }
        case OP:
        {
            switch (dif->data.op)
            {
                case ADD:
                    return ADD_ (DIF_ (dif->left), DIF_ (dif->right));
                case SUB:
                    return SUB_ (DIF_ (dif->left), DIF_ (dif->right));
                case MUL:
                    return ADD_ (MUL_ (DIF_ (dif->left), COPY_ (dif->right)), MUL_ (COPY_ (dif->left), DIF_ (dif->right)));
                case DIV:
                    return DIV_ (SUB_ (MUL_ (DIF_ (dif->left), COPY_ (dif->right)), MUL_ (COPY_ (dif->left), DIF_ (dif->right))), POW_ (COPY_ (dif->right), NUM_ (2)));
                case SIN:
                    return MUL_ (COS_ (COPY_ (dif->left)), DIF_ (dif->left));
                case COS:
                    return MUL_ (MUL_ (SIN_ (COPY_ (dif->left)), NUM_ (-1)), DIF_ (dif->left));
                case POW:
                    return MUL_ (MUL_ (POW_ (COPY_ (dif->left), NUM_ (dif->right->data.val - 1)), COPY_ (dif->right)), DIF_ (COPY_ (dif->left)));
                case SQRT:
                    return MUL_ (POW_ (COPY_ (dif->left), NUM_ (dif->right->data.val - 1)), COPY_ (dif->right));
                case LN:
                    return DIV_ (NUM_ (1), DIF_ (dif->left));
                default:
                {
                    printf ("Error in diffirentiation, op is %d", dif->data.op);
                    exit (-2);
                }
            }
        }
    }
}

NODE * CopyTree (NODE * root)
{
    my_assert (root);

    switch (root->node_type)
    {
        case OP:
        {
            NODE * left_node  = NULL;
            NODE * right_node = NULL;

            if (root->left)  left_node  = CopyTree (root->left);
            if (root->right) right_node = CopyTree (root->right);

            return NewOpNode (OP, root->data.op, left_node, right_node);
        }
        case VAR:
            return NewVarNode (VAR, root->data.var.var_name, NULL, NULL);
        case NUM:
            return NUM_ (root->data.val);
        default:
            return NULL;
    }
}

int GraphDump (NODE * node, const char * file_name)
{
    my_assert (node);
    my_assert (file_name);

    FILE * dump = fopen (file_name, "w+");
    my_assert (dump);

    fprintf (dump, "digraph differentiator{\n"
                    "\trankdir=LR;\n"
                    "\tbgcolor=\"#FFFFFF\";"
                    "\tnode[color=\"black\", fontsize=14];\n"
                    "\tfixedsize=true;\n");

    RecurcyDumpFill (dump, node);

    fprintf (dump, "}\n");

    my_assert (!fclose (dump));

    return 0;
}

int RecurcyDumpFill (FILE * file, NODE * node)
{
    my_assert (file);
    my_assert (node);

    const char * color = GOD_SAY_YES_COLOR;

    if (!node->left && !node->right)
        color = GOD_SAY_NO_COLOR;

    if (node->node_type == OP)
    {
        fprintf (file, "\tQ%p[style=filled, shape=record, fillcolor=\"%s\", width=2, label =\""
                   "{%c | {{left | %p} | {right | %p}}}\" ]\n",
                   node, color, node->data.op, node->left, node->right);
    }
    else if (node->node_type == NUM)
    {
        fprintf (file, "\tQ%p[style=filled, shape=record, fillcolor=\"%s\", width=2, label =\""
                "{%d | {{left | %p} | {right | %p}}}\" ]\n",
                node, color, node->data.val, node->left, node->right);
    }
    else if (node->node_type == VAR)
    {
        fprintf (file, "\tQ%p[style=filled, shape=record, fillcolor=\"%s\", width=2, label =\""
                "{%c | {{left | %p} | {right | %p}}}\" ]\n",
                node, color, node->data.var.var_name, node->left, node->right);
    }

    if (node->left)  RecurcyDumpFill (file, (NODE *) node->left);
    if (node->right) RecurcyDumpFill (file, (NODE *) node->right);

    if (node->left)
        fprintf (file, "\tQ%p -> Q%p\n", node, node->left);
    if (node->right)
        fprintf (file, "\tQ%p -> Q%p\n", node, node->right);
    fprintf (file, "\n");

    return 0;
}