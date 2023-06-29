#include "ast.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Node *nodeFactory(const enum NodeType nodeType, struct Pos *pos)
{
    struct Node *ret = malloc(sizeof(struct Node));
    ret->nodeType = nodeType;
    ret->pos = pos;
    ret->number_of_children = 0;
    ret->children = NULL;
    ret->data = stringKeyArbitraryValueMapFactory();

    return ret;
}

char *nodeToString(const struct Node *node)
{
    char *node_string = malloc(BUFF_SIZE);

    switch (node->nodeType)
    {
    case PROGRAM:
        strcpy(node_string, "PROGRAM");
        break;
    case FUNCTION:
        strcpy(node_string, "FUNCTION");
        break;
    case BODY:
        strcpy(node_string, "BODY");
        break;
    case RETURN:
        strcpy(node_string, "RETURN");
        break;
    case INT_LITERAL:
        strcpy(node_string, "INT_LITERAL");
        break;
    case UNARY_OP:
        strcpy(node_string, "UNARY_OP");
        break;
    case BIN_OP:
        strcpy(node_string, "BIN_OP");
        break;
    default:
        return "";
    }

    if (node->pos != NULL)
    {
        strcat(node_string, " ");
        char *pos_string = posToString(node->pos);
        strcat(node_string, pos_string);
        free(pos_string);
    }

    if (node->data->number_of_entries != 0)
    {
        strcat(node_string, " ");
        char *data_string = stringKeyArbitraryValueMapToString(node->data);
        strcat(node_string, data_string);
        free(data_string);
    }

    node_string = realloc(node_string, strlen(node_string) + 1);

    return node_string;
}

void printNode(const struct Node *node, const long indent)
{
    for (long i = 0; i < indent; i++)
    {
        printf("    ");
    }

    char *node_string = nodeToString(node);
    printf("%s", node_string);
    free(node_string);
    printf("\n");

    for (long i = 0; i < node->number_of_children; i++)
    {
        printNode(node->children[i], indent + 1);
    }
}

void nodeAddChild(struct Node *node, struct Node *node_to_add)
{

    if (node->number_of_children == 0)
    {
        node->children = malloc(sizeof(struct Node *));
    }
    else
    {
        node->children = realloc(node->children, sizeof(struct Node *) * (node->number_of_children + 1));
    }
    node->number_of_children++;
    node->children[node->number_of_children - 1] = node_to_add;
}

void nodeAddChildrenFromChild(struct Node *node, struct Node *node_children_to_add)
{
    for (long i = 0; i < node_children_to_add->number_of_children; i++)
    {
        nodeAddChild(node, node_children_to_add->children[i]);
    }
}

void nodePut(struct Node *node, struct StringKeyArbitraryValueMapEntry *value)
{
    stringKeyArbitraryValueMapAddItem(node->data, value);
}

void nodePutPreviousToken(struct Node *node, struct Token **tokens, long *pos, char *key)
{
    nodePut(node, stringKeyArbitraryValueMapEntryFactory(key, tokens[(*pos) - 1]->value));
}

struct ArbitraryValue *nodeGet(struct Node *node, const char *key)
{
    return stringKeyArbitraryValueMapGetItem(node->data, key);
}

void freeNode(struct Node *node)
{
    for (int i = 0; i < node->number_of_children; i++)
    {
        freeNode(node->children[i]);
    }
    // 'pos' is inherited from the 1st token it represents, therefore it is freed there
    freeNodeLessChildren(node);
}

void freeNodeLessChildren(struct Node *node)
{
    free(node->children);
    freeStringKeyArbitraryValueMap(node->data);
    free(node);
}

void printAst(const struct Ast *ast)
{
    printNode(ast->program, 0);
}

void freeAst(struct Ast *ast)
{
    freeNode(ast->program);
    free(ast);
}