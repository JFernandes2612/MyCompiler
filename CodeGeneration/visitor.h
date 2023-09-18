#ifndef VISITOR_H
#define VISITOR_H

#include "../Parser/ast.h"  
#include "../SemanticAnalysis/symbolTable.h"

char *codeGenerationLabel();

int codeGenerationVisitIntLiteral(struct Node *node, char *assemblyCode, const struct SymbolTable* symbolTable);

int codeGenerationVisitIdentifier(struct Node *node, char *assemblyCode, const struct SymbolTable* symbolTable);

int codeGenerationVisitUnaryOp(struct Node *node, char *assemblyCode, const struct SymbolTable* symbolTable);

int codeGenerationVisitBinOp(struct Node *node, char *assemblyCode, const struct SymbolTable* symbolTable);

int codeGenerationVisitReturn(struct Node *node, char *assemblyCode, const struct SymbolTable* symbolTable);

int codeGenerationVisitAttribution(struct Node *node, char *assemblyCode, const struct SymbolTable* symbolTable);

int codeGenerationVisitDeclaration(struct Node *node, char *assemblyCode, const struct SymbolTable* symbolTable);

int codeGenerationVisitFunction(struct Node *node, char *assemblyCode, const struct SymbolTable* symbolTable);

int codeGenerationVisitProgram(struct Node *node, char *assemblyCode, const struct SymbolTable* symbolTable);

int codeGenerationVisitDown(struct Node *node, char *assemblyCode, const struct SymbolTable* symbolTable);

int codeGenerationVisit(struct Node *node, char *assemblyCode, const struct SymbolTable* symbolTable);

char *codeGeneration(struct Ast *ast, const struct SymbolTable* symbolTable);

#endif