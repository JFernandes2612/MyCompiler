#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

struct SymbolTable
{
    char **varNames;
    long numberOfVars;
};

struct SymbolTable *symbolTableFactory();

void symbolTableAddVariable(struct SymbolTable *symbolTable, char *varName);

long symbolTableGetVariableOffset(const struct SymbolTable *symbolTable, const char *varName);

void printSymbolTable(const struct SymbolTable *symbolTable);

void freeSymbolTable(struct SymbolTable *symbolTable);

#endif