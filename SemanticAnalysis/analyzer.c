#include "analyzer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct SymbolTable *analyze(const struct Ast *ast)
{
    struct SymbolTable *symbolTable = symbolTableFactory();

    if (analyzerVisit(ast->program, symbolTable))
    {
        freeSymbolTable(symbolTable);
        return NULL;
    }

    return symbolTable;
}

int analyzerVisit(struct Node *node, struct SymbolTable *symbolTable)
{
    switch (node->nodeType)
    {
    case FUNCTION:
        return analyzerVisitFunction(node, symbolTable);
        break;
    case DECLARATION:
        return analyzerDeclaration(node, symbolTable);
        break;
    default:
        return analyzerVisitDown(node, symbolTable);
        break;
    }

    return 0;
}

int analyzerVisitDown(struct Node *node, struct SymbolTable *symbolTable)
{
    for (long i = 0; i < node->number_of_children; i++)
    {
        if (analyzerVisit(node->children[i], symbolTable))
        {
            freeSymbolTable(symbolTable);
            return -1;
        }
    }

    return 0;
}

int analyzerVisitFunction(struct Node *node, struct SymbolTable *symbolTable)
{
    //! Check if the only function is the main function - will be changed to use symbol table
    char *func_name = arbitraryValueToString(stringKeyArbitraryValueMapGetItem(node->data, "funcName"));
    if (strcmp(func_name, "main"))
    {
        printf("No main function found\n");
        freeSymbolTable(symbolTable);
        return -1;
    }

    free(func_name);

    return analyzerVisitDown(node, symbolTable);
}

int analyzerDeclaration(struct Node *node, struct SymbolTable *symbolTable)
{
    char* variable_name = arbitraryValueToString(stringKeyArbitraryValueMapGetItem(node->data, "id"));
    symbolTableAddVariable(symbolTable, variable_name);

    return analyzerVisitDown(node, symbolTable);
}