#ifndef ANALYZER_H
#define ANALYZER_H

#include "../Parser/ast.h"
#include "symbolTable.h"

struct SymbolTable *analyze(const struct Ast *ast);

int analyzerVisit(struct Node *node, struct SymbolTable *symbolTable);

int analyzerVisitDown(struct Node *node, struct SymbolTable *symbolTable);

int analyzerDeclaration(struct Node *node, struct SymbolTable *symbolTable);

int analyzerVisitFunction(struct Node *node, struct SymbolTable *symbolTable);

#endif