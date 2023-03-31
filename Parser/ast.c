#include "ast.h"
#include <stdlib.h>

const struct Node nodeFactory(const enum NodeType nodeType)
{
    struct Node ret;
    ret.number_of_children = 0;
    ret.nodeType = nodeType;
    ret.children = NULL;

    return ret;
}

void addChild(struct Node *node)
{
    if (node->number_of_children == 0)
    {
        node->children = malloc(sizeof(struct Node));
        node->children[0] = *node;
    }
    else
    {
        node->number_of_children++;
        node->children = realloc(node->children, sizeof(struct Node) * node->number_of_children);
        node->children[node->number_of_children - 1] = *node;
    }
}