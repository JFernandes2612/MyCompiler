#ifndef ANALYZER_H
#define ANALYZER_H

#include "../Parser/ast.h"

int analyze(const struct Ast *ast, int (**f)(const struct Ast *), long number_of_analyzers);

int analizeOnlyOneMainFunction(const struct Ast *ast);

#endif