#include "../inc/Differentiator.h"

static nodeElem GLOBAL_X = 0;

const char * GOD_SAY_NO_COLOR   = "#FF7B61";
const char * GOD_SAY_YES_COLOR  = "#B2EC5D";

const char * READ_DUMP          = "./logs/FirstGraphDump.dot";
const char * DIFF_DUMP          = "./logs/DiffGraphDump.dot";

int main ()
{
    NODE * node = GetG ();

    printf ("Answer = %d\n", Eval (node));

    NODE * dif_node = StartDiff (node);

    GraphDump (dif_node, DIFF_DUMP);

    GraphDump (node, READ_DUMP);

    system ("dot -Tsvg ./logs/FirstGraphDump.dot -o ./logs/FirstGraphDump.svg");
    system ("dot -Tsvg ./logs/DiffGraphDump.dot -o ./logs/DiffGraphDump.svg");

    system ("ren logs/FirstGraphDump.svg FirstGraphDump.html");
    system ("ren logs/DiffGraphDump.svg DiffGraphDump.html");
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

int TreeDtor (NODE * node)
{
    my_assert (node);

    if (node->left)  RecurcyDtor (node->left);
    if (node->right) RecurcyDtor (node->right);

    node->left = NULL;
    node->right = NULL;

    node->data.op = NOT_OPERATOR;
    node->data.val = 0;
    node->data.var.var_name = 0;
    node->data.var.var_val = 0;

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

nodeElem Eval (NODE * node)
{
    my_assert (node);

    if (node->node_type == NUM) 
        return node->data.val;
    else if (node->node_type == VAR)
        return GLOBAL_X;
    else if (node->node_type = OP)
        switch (node->data.op)
        {
            case ADD:
                return Eval (node->left) + Eval (node->right);
            case SUB:
                return Eval (node->left) - Eval (node->right);
            case MUL:
                return Eval (node->left) * Eval (node->right);
            case DIV:
                return Eval (node->left) / Eval (node->right);
            case SIN:
                return sin (Eval (node->left));
            case COS:
                return cos (Eval (node->left));
            case LN:
                return log (Eval (node->left));
            case POW:
            {
                double res = pow ((double) Eval (node->left), (double) Eval (node->right));

                return res;
            }
            case SQRT:
                return pow (Eval (node->left), 0.5);
            default:
            {
                printf ("Error while calculating %d\n", node->node_type);
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

NODE * StartDiff (NODE * dif)
{
    my_assert (dif);

    BufferCleaner ();

    printf ("Please, enter the variable by which to differentiate: ");

    char var_name = NULL;

    while (scanf ("%c", &var_name) != 1)
    {
        printf ("Please, enter the variable\n");

        BufferCleaner ();
    }

    printf ("\n");

    NODE * differentiared = Diffirentiation (dif, &var_name);

    NODE * simply = NumsSumSimplification (differentiared);

    if (simply)
        return simply;
    else
        return differentiared;
}

NODE * Diffirentiation (NODE * dif, char * var)
{
    my_assert (dif);

    switch (dif->node_type)
    {
        case NUM:
            return NewNumNode (NUM, 0, NULL, NULL);
        case VAR:
        {
            if (dif->data.var.var_name == *var)
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
                    return MUL_ (MUL_ (COS_ (DIF_ (dif->left)), DIF_ (dif->right)), DIF_ (dif->right));
                case COS:
                    return MUL_ (MUL_ (SIN_ (DIF_ (dif->left)), NUM_ (-1)), DIF_ (dif->right));
                case POW:
                    return MUL_ (MUL_ (POW_ (COPY_ (dif->left), NUM_ (dif->right->data.val - 1)), COPY_ (dif->right)), DIF_ (COPY_ (dif->left)));
                // case SQRT:
                //     return MUL_ (POW_ (COPY_ (dif->left), NUM_ (dif->right->data.val - 1)), COPY_ (dif->right));
                // case LN:
                //     return LN_ (DIF_ (dif->left));
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

            if (root->left) left_node = CopyTree (root->left);
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

NODE * NumsSumSimplification (NODE * node)
{
    my_assert (node);

    if (node->node_type == NUM) 
        return node;
    else if (node->node_type == VAR) 
        return NULL;
    else
    {
        NODE * a = NumsSumSimplification (node->left);
        NODE * b = NumsSumSimplification (node->right);

        if (!a || !b)
            return NULL;
        
        NODE * new_node = NUM_ (a->data.val + b->data.val);

        TreeDtor (node);

        return new_node;
    }
}

NODE * OneSimplification (NODE * node)
{
    my_assert (node);

    NODE * a = NULL;
    NODE * b = NULL;

    if (node->left && node->right)
    {
        a = OneSimplification (node->left);
        b = OneSimplification (node->right);
    }
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