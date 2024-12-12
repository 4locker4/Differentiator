#include "../inc/Differentiator.h"

NODE * MakeGraphSimply (NODE * node)
{
    my_assert (node);

    NODE * res = OneSimplification (node);

    res = NumsSumSimplification (res);

    return res;
}

NODE * NumsSumSimplification (NODE * node)
{
    my_assert (node);

    if (node->left)
        node->left = NumsSumSimplification (node->left);
    else
        return node;
    if (node->right)
        node->right = NumsSumSimplification (node->right);
    else
        return node;

    if (node->node_type == OP && node->left->node_type == NUM && node->right->node_type == NUM) 
    {       
        NODE * new_node = NUM_ (Eval (node, 0));

        TreeDtor (node, ALL_NODES);

        return new_node;
    }
    else
        return node;
}

NODE * OneSimplification (NODE * node)
{
    my_assert (node);

    if (node->left)
        node->left = OneSimplification (node->left);
    else
        return node;
    if (node->right)
        node->right = OneSimplification (node->right);
    else
        return node;

    if (node->node_type == OP)
    {
        switch (node->data.op)
        {
            case MUL:
            {
                if (node->left->node_type == NUM)
                {
                    if (node->left->data.val == 1)
                        L_DTOR (node)
                    else if (node->left->data.val == 0)
                        R_DTOR (node)
                }
                else if (node->right->node_type == NUM)
                {
                    if (node->right->data.val == 1)
                        R_DTOR (node)
                    else if (node->right->data.val == 0)
                        L_DTOR (node)
                }
            }
            case DIV:
            {
                if (node->left->node_type == NUM)
                {
                    if (node->left->data.val == 1)
                        return node;
                    else if (node->left->data.val == 0)
                        R_DTOR (node)
                }
                else if (node->right->node_type == NUM)
                {
                    if (node->right->data.val == 1)
                        R_DTOR (node)
                    else if (node->right->data.val == 0)
                    {
                        printf ("Error, somethinng / 0!\n");

                        abort ();

                        return 0;
                    }
                }
            }
            case POW:
            {
                if (node->left->node_type == NUM)
                {
                    if (node->left->data.val == 1)
                        L_DTOR (node)
                    else if (node->left->data.val == 0)
                        R_DTOR (node)
                }
                else if (node->right->node_type == NUM)
                {
                    if (node->right->data.val == 1)
                        R_DTOR (node)
                    else if (node->right->data.val == 0)
                    {
                        TreeDtor (node, ALL_NODES);

                        return NUM_ (1);
                    }
                }
            }
            case ADD:
            {
                if (node->left->node_type == NUM)
                {
                    if (node->left->data.val == 0)
                        L_DTOR (node)
                }
                else if (node->right->node_type == NUM)
                {
                    if (node->right->data.val == 0)
                        R_DTOR (node)
                }
            }
            case SUB:
            {
                if (node->left->node_type == NUM)
                {
                    if (node->left->data.val == 0)
                        L_DTOR (node)
                }
                else if (node->right->node_type == NUM)
                {
                    if (node->right->data.val == 0)
                        R_DTOR (node)
                }
            }
            default:
                return node;
        }
    }

    return node;
}