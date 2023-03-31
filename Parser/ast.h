#ifndef AST_H
#define AST_H

enum NodeType
{
    PROGRAM,

    INT_KEYWORD,
    RETURN_KEYWORD,
    IDENTIFIER,
    INT_LITERAL,

    NONE,
};

struct Node
{
    enum NodeType nodeType;

    long number_of_children;
    struct Node *children;
};

const struct Node nodeFactory(const enum NodeType nodeType);

void addChild(struct Node *node);

struct Ast
{
    struct Node program;
};

#endif