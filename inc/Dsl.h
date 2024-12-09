#ifndef DSL_H
#define DSL_H

#include "Differentiator.h"

#define NUM_(num) NewNumNode (NUM, num, NULL, NULL)

#define ADD_(node_1, node_2)  NewOpNode (OP, ADD, node_1, node_2)
#define SUB_(node_1, node_2)  NewOpNode (OP, SUB, node_1, node_2)
#define MUL_(node_1, node_2)  NewOpNode (OP, MUL, node_1, node_2)
#define DIV_(node_1, node_2)  NewOpNode (OP, DIV, node_1, node_2)
#define POW_(node_1, node_2)  NewOpNode (OP, POW, node_1, node_2)
#define SIN_(node)            NewOpNode (OP, SIN, node, NULL)
#define COS_(node)            NewOpNode (OP, COS, node, NULL)
#define SQRT_(node)           NewOpNode (OP, SQRT, node, NULL)
#define LN_(node)             NewOpNode (OP, LN, node, NULL)

#define DIF_(dif)             Diffirentiation (dif, var)

#define COPY_(root)           CopyTree (root)

#endif