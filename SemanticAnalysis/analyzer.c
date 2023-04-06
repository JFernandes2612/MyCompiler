#include "analyzer.h"
#include <string.h>
#include <stdio.h>

int analyze(const struct Ast *ast, int (**f)(const struct Ast *), long number_of_analyzers)
{
    for (long i = 0; i < number_of_analyzers; i++)
    {
        if ((*f[0])(ast))
        {
            return -1;
        }
    }

    return 0;
}

int analizeOnlyOneMainFunction(const struct Ast *ast)
{
    int foundFirst = 0;

    for (long i = 0; i < ast->program->number_of_children; i++)
    {
        struct Node *node = ast->program->children[i];

        if (node->nodeType == FUNCTION)
        {
            int isMainFunction = strcmp(arbitraryValueToString(stringKeyArbitraryValueMapGetItem(node->data, "funcName")), "main") == 0;

            if (!foundFirst && isMainFunction)
            {
                foundFirst = 1;
            }
            else if (isMainFunction)
            {
                printf("Semantic Error: Main function re-declaration at %s\n", posToString(node->pos));
                return -1;
            }
        }
    }

    if (!foundFirst)
    {
        printf("Semantic Error: Main function not found\n");
        return -1;
    }

    return 0;
}