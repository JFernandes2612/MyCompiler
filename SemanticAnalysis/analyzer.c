#include "analyzer.h"
#include <string.h>
#include <stdio.h>

int analyze(const struct Ast *ast)
{
    return analyzerVisit(ast->program);
}


int analyzerVisit(struct Node *node)
{
    switch (node->nodeType)
    {
    case FUNCTION:
        return analyzerFunction(node);
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

int analyzerFunction(struct Node *node)
{
    // Check if the only function is the main function
    if (strcmp(arbitraryValueToString(stringKeyArbitraryValueMapGetItem(node->data, "funcName")), "main"))
    {
        printf("No main function found\n");
        return -1;
    }

    return analyzerVisitDown(node);
}