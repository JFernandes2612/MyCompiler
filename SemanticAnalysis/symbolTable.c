#include "symbolTable.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct SymbolTable *symbolTableFactory()
{
    struct SymbolTable *ret = malloc(sizeof(struct SymbolTable));

    ret->varNames = NULL;
    ret->numberOfVars = 0;

    return ret;
}

void symbolTableAddVariable(struct SymbolTable *symbolTable, char *varName)
{
    symbolTable->numberOfVars++;

    if (symbolTable->varNames == NULL)
    {
        symbolTable->varNames = malloc(sizeof(char *));
    }
    else
    {
        symbolTable->varNames = realloc(symbolTable->varNames, sizeof(char *) * symbolTable->numberOfVars);
    }

    symbolTable->varNames[symbolTable->numberOfVars - 1] = varName;
}

long symbolTableGetVariableOffset(const struct SymbolTable *symbolTable, const char *varName)
{
    for (int i = 0; i < symbolTable->numberOfVars; i++)
    {
        if (strcmp(symbolTable->varNames[i], varName) == 0)
            return (i + 1) * -8;
    }

    return -1;
}

void printSymbolTable(const struct SymbolTable *symbolTable)
{
    for (int i = 0; i < symbolTable->numberOfVars; i++)
    {
        printf("%s\n", symbolTable->varNames[i]);
    }
}

void freeSymbolTable(struct SymbolTable *symbolTable)
{
    for (int i = 0; i < symbolTable->numberOfVars; i++)
    {
        free(symbolTable->varNames[i]);
    }

    free(symbolTable->varNames);

    free(symbolTable);
}