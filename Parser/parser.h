#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "../Lexer/token.h"

int testRule(struct Node *root, struct Token **tokens, long *pos, const enum NodeType node_type);

int testToken(struct Token **tokens, long *pos, const enum TokenType token_type);

int testTokens(struct Token **tokens, long *pos, const enum TokenType *token_types, const long number_of_tests);

int buildRule(struct Node *root, struct Token **tokens, long *pos);

int buildProgram(struct Node *root, struct Token **tokens, long *pos);

int buildFunction(struct Node *root, struct Token **tokens, long *pos);

int buildBody(struct Node *root, struct Token **tokens, long *pos);

int buildReturn(struct Node *root, struct Token **tokens, long *pos);

int buildIntLiteral(struct Node *root, struct Token **tokens, long *pos);

struct Ast *parse(struct Token **tokens);

#endif