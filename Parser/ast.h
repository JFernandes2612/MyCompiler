#ifndef AST_H
#define AST_H

enum NodeType
{
    PROGRAM,
    FUNCTION,
    BODY,
    RETURN,

    INT_LITERAL,
};

struct Node
{
    enum NodeType nodeType;

    long number_of_children;
    struct Node *children;
};

const struct Node nodeFactory(const enum NodeType nodeType);

void addChild(struct Node *node, struct Node node_to_add);

const char *nodeToString(const struct Node node);

void printNode(const struct Node node, const long indent);

struct Ast
{
    struct Node program;
};

void printAst(const struct Ast ast);

#endif