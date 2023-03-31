#include "ast.h"
#include <stdlib.h>
#include <stdio.h>

const struct Node nodeFactory(const enum NodeType nodeType)
{
    struct Node ret;
    ret.number_of_children = 0;
    ret.nodeType = nodeType;
    ret.children = NULL;

    return ret;
}

const char *nodeToString(const struct Node node)
{
    switch (node.nodeType)
    {
    case PROGRAM:
        return "PROGRAM";
        break;
    case FUNCTION:
        return "FUNCTION";
        break;
    case BODY:
        return "BODY";
        break;
    case RETURN:
        return "RETURN";
        break;
    }

    return "";
}

void printNode(const struct Node node, const long indent)
{
    for (long i = 0; i < indent; i++)
    {
        printf("\t");
    }

    printf(nodeToString(node));
    printf("\n");

    for (long i = 0; i < node.number_of_children; i++)
    {
        printNode(node.children[i], indent + 1);
    }
}

void addChild(struct Node *node, struct Node node_to_add)
{

    if (node->number_of_children == 0)
    {
        node->children = malloc(sizeof(struct Node));
    }
    else
    {
        node->children = realloc(node->children, sizeof(struct Node) * (node->number_of_children + 1));
    }
    node->number_of_children++;
    node->children[node->number_of_children - 1] = node_to_add;
}

void printAst(const struct Ast ast)
{
    printNode(ast.program, 0);
}