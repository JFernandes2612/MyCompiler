#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "../Lexer/token.h"

int addChildToRootAst(struct Node *root, const struct Token *tokens, long *pos, const enum NodeType node_type);

int testToken(const struct Token *tokens, long *pos, enum TokenType token_type);

int testTokens(const struct Token *tokens, long *pos, const enum TokenType *token_types, const long number_of_tests);

int buildRule(struct Node *root, const struct Token *tokens, long *pos);

int buildProgram(struct Node *root, const struct Token *tokens, long *pos);

int buildFunction(struct Node *root, const struct Token *tokens, long *pos);

int buildBody(struct Node *root, const struct Token *tokens, long *pos);

int buildReturn(struct Node *root, const struct Token *tokens, long *pos);

int buildIntLiteral(struct Node *root, const struct Token *tokens, long *pos);

const struct Ast *parse(const struct Token *tokens);

#endif