#include "analyzer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int analyze(const struct Ast *ast)
{
    return analyzerVisit(ast->program);
}

int analyzerVisit(struct Node *node)
{
    switch (node->nodeType)
    {
    case FUNCTION:
        return analyzerVisitFunction(node);
        break;
    default:
        return analyzerVisitDown(node);
        break;
    }

    return 0;
}

int analyzerVisitDown(struct Node *node)
{
    for (long i = 0; i < node->number_of_children; i++)
    {
        if (analyzerVisit(node->children[i]))
            return -1;
    }

    return 0;
}

int analyzerVisitFunction(struct Node *node)
{
    //! Check if the only function is the main function - will be changed to use symbol table
    char *func_name = arbitraryValueToString(stringKeyArbitraryValueMapGetItem(node->data, "funcName"));
    if (strcmp(func_name, "main"))
    {
        printf("No main function found\n");
        return -1;
    }

    free(func_name);

    return analyzerVisitDown(node);
}