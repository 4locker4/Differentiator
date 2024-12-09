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
    SIN,
    COS,
    LN,
    SQRT,
    POW = '^',
    ADD = '+',
    SUB = '-',
    MUL = '*',
    DIV = '/',
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

    NODE * root   = NULL;

    bool   status = 0;
} TREE;

#define FILL_THE_NODE_DATA_(type, elem, node)                                                             \
                                        {                                                                 \
                                            if (type == OP)  node->data.op  = (OPERATORS) elem;           \
                                            if (type == NUM) node->data.val = elem;                       \
                                        }
int      SyntaxError            ();
NODE *   GetG                   ();
NODE *   GetExpression          ();
NODE *   GetT                   ();
NODE *   GetDegree              ();
NODE *   GetP                   ();
NODE *   GetN                   ();
int      GraphDump              (NODE * node, const char * file_name);
int      RecurcyDumpFill        (FILE * file, NODE * node);
NODE *   NewNumNode             (TYPES type, int elem, NODE * left_node, NODE * right_node);
NODE *   NewVarNode             (TYPES type, char var, NODE * left_node, NODE * right_node);
NODE *   NewOpNode              (TYPES type, OPERATORS op, NODE * left_node, NODE * right_node);
nodeElem Eval                   (NODE * node);
NODE *   Diffirentiation        (NODE * dif, char * var);
NODE *   StartDiff              (NODE * dif);
NODE *   CopyTree               (NODE * root);
NODE *   NumsSumSimplification  (NODE * node);
int      RecurcyDtor            (NODE * node);
int      TreeDtor               (NODE * node);

#endif