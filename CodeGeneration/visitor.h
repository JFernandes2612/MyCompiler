#ifndef VISITOR_H
#define VISITOR_H

#include "../Parser/ast.h"

void codeGenerationVisitIntLiteral(struct Node *node, char *assemblyCode);

void codeGenerationVisitUnaryOp(struct Node *node, char *assemblyCode);

void codeGenerationVisitReturn(struct Node *node, char *assemblyCode);

void codeGenerationVisitFunction(struct Node *node, char *assemblyCode);

void codeGenerationVisitProgram(struct Node *node, char *assemblyCode);

void codeGenerationVisitDown(struct Node *node, char *assemblyCode);

void codeGenerationVisit(struct Node *node, char *assemblyCode);

char *codeGeneration(struct Ast *ast);

#endif