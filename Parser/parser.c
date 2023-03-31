#include "parser.h"
#include <stdlib.h>

const struct Node buildProgram(struct Node *node, const struct Token *tokens, long *pos)
{
    struct Node program = nodeFactory(PROGRAM);

    return program;
}

const struct Ast *parse(const struct Token *tokens)
{
    struct Ast *ast = malloc(sizeof(struct Ast));

    struct Node program = nodeFactory(PROGRAM);
    long pos = 0;

    ast->program = buildProgram(&program, tokens, &pos);

    return ast;
}