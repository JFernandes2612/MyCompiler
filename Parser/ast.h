#ifndef AST_H
#define AST_H

#include "../Utils/utils.h"
#include "../Lexer/token.h"

enum NodeType
{
    PROGRAM,
    FUNCTION,
    BODY,
    EXPRESSION,
    UNARY_OP,
    BIN_OP,
    RETURN,

    INT_LITERAL,
};

struct Node
{
    enum NodeType nodeType;
    struct Pos *pos;

    long number_of_children;
    struct Node **children;
    struct StringKeyArbitraryValueMap *data;
};

struct Node *nodeFactory(const enum NodeType nodeType, struct Pos *pos);

void nodeAddChild(struct Node *node, struct Node *node_to_add);

void nodeAddChildrenFromChild(struct Node *node, struct Node *node_children_to_add);

void nodePut(struct Node *node, struct StringKeyArbitraryValueMapEntry *value);

void nodePutPreviousToken(struct Node *node, struct Token **tokens, long *pos, char *key);

struct ArbitraryValue *nodeGet(struct Node *node, const char *key);

const char *nodeToString(const struct Node *node);

void printNode(const struct Node *node, const long indent);

void freeNode(struct Node *node);

struct Ast
{
    struct Node *program;
};

void printAst(const struct Ast *ast);

void freeAst(struct Ast *ast);

#endif