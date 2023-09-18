#ifndef VISITOR_H
#define VISITOR_H

#include "../Parser/ast.h"

char *codeGenerationLabel();

int codeGenerationVisitIntLiteral(struct Node *node, char *assemblyCode);

int codeGenerationVisitIdentifier(struct Node *node, char *assemblyCode);

int codeGenerationVisitUnaryOp(struct Node *node, char *assemblyCode);

int codeGenerationVisitBinOp(struct Node *node, char *assemblyCode);

int codeGenerationVisitReturn(struct Node *node, char *assemblyCode);

int codeGenerationVisitAttribution(struct Node *node, char *assemblyCode);

int codeGenerationVisitDeclaration(struct Node *node, char *assemblyCode);

int codeGenerationVisitFunction(struct Node *node, char *assemblyCode);

int codeGenerationVisitProgram(struct Node *node, char *assemblyCode);

int codeGenerationVisitDown(struct Node *node, char *assemblyCode);

int codeGenerationVisit(struct Node *node, char *assemblyCode);

char *codeGeneration(struct Ast *ast);

#endif