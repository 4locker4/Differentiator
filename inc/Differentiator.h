#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "Utils.h"
#include "Asserts.h"
#include "Dsl.h"

typedef int nodeElem;

const int POISON = 422;

enum TYPES
{
    NOT_INITED = 0,
    VAR,
    NUM,
    OP
};

enum OPERATORS
{
    NOT_OPERATOR = 0,
    SIN  = 's',
    COS  = 'c',
    LN   = 'l',
    SQRT = 'S',
    POW  = '^',
    ADD  = '+',
    SUB  = '-',
    MUL  = '*',
    DIV  = '/',
};

enum WHAT_DTOR
{
    ALL_NODES,
    WITH_LEFT_BRACH,
    WITH_RIGHT_BRANCH
};

typedef struct
{
    int  var_val  = 0;
    char var_name = 0;
} VAR_DATA;

typedef union
{
    VAR_DATA  var;
    nodeElem  val;
    OPERATORS op;
} NODE_DATA;

typedef struct NODE
{
    struct NODE * left;
    struct NODE * right;

    struct NODE * parent;

    TYPES node_type = NOT_INITED;
    NODE_DATA data  = {};
} NODE;

typedef struct
{
    const char * data_base_file = NULL;

    NODE * root             = NULL;
    NODE * diffed_expr_root = NULL;
    NODE * taylor_series    = NULL;
    char * expr             = NULL;

    size_t tree_size    = 0;
    int    text_pointer = 0;
    int    depth        = 0;

    bool   status    = 0;
} TREE;

#define R_DTOR(node)                                            \
                    {                                           \
                        NODE * new_node = node->left;           \
                                                                \
                        TreeDtor (node, WITH_RIGHT_BRANCH);     \
                                                                \
                        return new_node;                        \
                    }

#define L_DTOR(node)                                            \
                    {                                           \
                        NODE * new_node = node->right;          \
                                                                \
                        TreeDtor (node, WITH_LEFT_BRACH);       \
                                                                \
                        return new_node;                        \
                    }

TREE *   TreeCtor                   ();
int      TreeDtor                   (NODE * node, WHAT_DTOR branch);
int      SyntaxError                (char * expr, int * pointer);
NODE *   GetG                       (char * expr, int * pointer);
NODE *   GetExpression              (char * expr, int * pointer);
NODE *   GetT                       (char * expr, int * pointer);
NODE *   GetDegree                  (char * expr, int * pointer);
NODE *   GetFunc                    (char * expr, int * pointer);
NODE *   GetP                       (char * expr, int * pointer);
NODE *   GetN                       (char * expr, int * pointer);
int      GraphDump                  (NODE * node, const char * file_name);
int      RecurcyDumpFill            (FILE * file, NODE * node);
NODE *   NewNumNode                 (TYPES type, int elem, NODE * left_node, NODE * right_node);
NODE *   NewVarNode                 (TYPES type, char var, NODE * left_node, NODE * right_node);
NODE *   NewOpNode                  (TYPES type, OPERATORS op, NODE * left_node, NODE * right_node);
nodeElem Eval                       (NODE * node, int x0);
NODE *   Diffirentiation            (NODE * dif, char var);
NODE *   StartDiff                  (TREE * tree);
NODE *   CopyTree                   (NODE * root);
int      RecurcyDtor                (NODE * node);
NODE *   NumsSumSimplification      (NODE * node);
NODE *   OneSimplification          (NODE * node);
NODE *   MakeGraphSimply            (NODE * node);
NODE *   TakeHigherOrderDerivative  (NODE * node, char var_name, int * depth, int max_degree);
NODE *   Taylor                     (TREE * tree);
NODE *   TaylorRecursy              (NODE * node, char var_name, int * depth, int max_depth, int x0);

#endif