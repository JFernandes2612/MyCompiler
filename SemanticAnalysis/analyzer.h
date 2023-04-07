#ifndef ANALYZER_H
#define ANALYZER_H

#include "../Parser/ast.h"

int analyze(const struct Ast *ast);

int analyzerVisit(struct Node *node);

int analyzerVisitDown(struct Node *node);

int analyzerFunction(struct Node *node);

#endif