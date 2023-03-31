#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "../Lexer/token.h"

const struct Node buildProgram(struct Node *node, const struct Token *tokens, long *pos);

const struct Ast *parse(const struct Token *tokens);

#endif