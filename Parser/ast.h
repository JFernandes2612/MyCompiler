#ifndef AST_H
#define AST_H

enum NodeType
{
    PROGRAM,

    INT_KEYWORD,
    RETURN_KEYWORD,
    IDENTIFIER,
    INT_LITERAL,
};

struct Node
{
    enum NodeType nodeType;

    long number_of_children;
    struct Node *children;
};

struct Ast
{
    struct Node program;
};

#endif