#include "../inc/Differentiator.h"

const char * TEILOR_DATA        = "./inc/Teilor.txt";

const char * TAYLOR_DUMP        = "./logs/TaylorGraphDump.dot";
const char * READ_DUMP          = "./logs/FirstGraphDump.dot";
const char * DIFF_DUMP          = "./logs/DiffGraphDump.dot";

int main ()
{
    TREE * tree = TreeCtor ();

    tree->root = MakeGraphSimply (tree->root);

    GraphDump (tree->root, READ_DUMP);

    system ("dot -Tsvg ./logs/FirstGraphDump.dot -o ./logs/FirstGraphDump.svg");

    tree->diffed_expr_root = MakeGraphSimply (tree->diffed_expr_root);

    GraphDump (tree->diffed_expr_root, DIFF_DUMP);

    system ("dot -Tsvg ./logs/DiffGraphDump.dot -o ./logs/DiffGraphDump.svg");

    Taylor (tree);

    GraphDump (tree->taylor_series, TAYLOR_DUMP);

    system ("dot -Tsvg ./logs/TaylorGraphDump.dot -o ./logs/TaylorGraphDump.svg");

    system ("ren logs/TaylorGraphDump.svg TaylorGraphDump.html");
    system ("ren logs/FirstGraphDump.svg FirstGraphDump.html");
    system ("ren logs/DiffGraphDump.svg DiffGraphDump.html");
}