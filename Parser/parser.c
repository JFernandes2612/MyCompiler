#include "parser.h"
#include <stdlib.h>
#include <stdio.h>

int voidRule(const enum NodeType node_type)
{
    return node_type == EXPRESSION;
}

int testRule(struct Node *root, struct Token **tokens, long *pos, const enum NodeType node_type)
{
    struct Node *functionNode = nodeFactory(node_type, tokens[*pos]->pos);
    long init_pos = *pos;
    if (buildRule(functionNode, tokens, pos))
    {
        (*pos) = init_pos;
        freeNode(functionNode);
        return -1;
    }

    if (voidRule(node_type))
        nodeAddChildrenFromChild(root, functionNode);
    else
        nodeAddChild(root, functionNode);

    return 0;
}

int testRules(struct Node *root, struct Token **tokens, long *pos, const enum NodeType *node_types, const long number_of_tests)
{
    long init_pos = *pos;
    for (long i = 0; i < number_of_tests; i++)
    {
        if (testRule(root, tokens, pos, node_types[i]))
        {
            (*pos) = init_pos;
            return -1;
        }
    }

    return 0;
}

int testToken(struct Token **tokens, long *pos, const enum TokenType token_type)
{
    const struct Token *token = tokens[*pos];

    if (token->token_type != token_type)
    {
        printf("Expected '%s' found '%s'\n", tokenToString(tokenFactory(token_type, NULL, NULL)), tokenToString(token));
        return -1;
    }

    (*pos)++;

    return 0;
}

int testIfToken(struct Token **tokens, long *pos, const enum TokenType token_type)
{
    const struct Token *token = tokens[*pos];

    if (token->token_type != token_type)
    {
        return -1;
    }

    (*pos)++;

    return 0;
}

int testTokens(struct Token **tokens, long *pos, const enum TokenType *token_types, const long number_of_tests)
{
    for (long i = 0; i < number_of_tests; i++)
    {
        if (testToken(tokens, pos, token_types[i]))
        {
            return -1;
        }
    }

    return 0;
}

int buildProgram(struct Node *root, struct Token **tokens, long *pos)
{
    if (testRule(root, tokens, pos, FUNCTION))
    {
        return -1;
    }

    if (testToken(tokens, pos, EOF_T))
    {
        return -1;
    }

    return 0;
}

int buildFunction(struct Node *root, struct Token **tokens, long *pos)
{
    const enum TokenType tokens_to_test_1[2] = {INT_KEYWORD_T, IDENTIFIER_T};

    if (testTokens(tokens, pos, tokens_to_test_1, 2))
    {
        return -1;
    }

    nodePutPreviousToken(root, tokens, pos, "funcName");

    const enum TokenType tokens_to_test_2[3] = {OPEN_PAREN_T, CLOSE_PAREN_T, OPEN_BRACE_T};

    if (testTokens(tokens, pos, tokens_to_test_2, 3))
    {
        return -1;
    }

    if (testRule(root, tokens, pos, BODY))
    {
        return -1;
    }

    if (testToken(tokens, pos, CLOSE_BRACE_T))
    {
        return -1;
    }

    return 0;
}

int buildBody(struct Node *root, struct Token **tokens, long *pos)
{
    if (testRule(root, tokens, pos, RETURN))
    {
        return -1;
    }

    return 0;
}

int buildReturn(struct Node *root, struct Token **tokens, long *pos)
{
    if (testToken(tokens, pos, RETURN_KEYWORD_T))
    {
        return -1;
    }

    if (testRule(root, tokens, pos, EXPRESSION))
    {
        return -1;
    }

    if (testToken(tokens, pos, SEMICOLON_T))
    {
        return -1;
    }

    return 0;
}

int buildExpression(struct Node *root, struct Token **tokens, long *pos)
{
    if (testRule(root, tokens, pos, INT_LITERAL) && testRule(root, tokens, pos, UNARY_OP))
    {
        return -1;
    }

    return 0;
}

int buildUnaryOp(struct Node *root, struct Token **tokens, long *pos)
{
    if (testToken(tokens, pos, MINUS_T))
    {
        return -1;
    }

    nodePutPreviousToken(root, tokens, pos, "op");

    if (testRule(root, tokens, pos, EXPRESSION))
    {
        return -1;
    }

    return 0;
}

int buildIntLiteral(struct Node *root, struct Token **tokens, long *pos)
{
    if (testToken(tokens, pos, INT_LITERAL_T))
    {
        return -1;
    }

    nodePutPreviousToken(root, tokens, pos, "value");

    return 0;
}

int buildRule(struct Node *root, struct Token **tokens, long *pos)
{
    switch (root->nodeType)
    {
    case PROGRAM:
        return buildProgram(root, tokens, pos);
        break;
    case FUNCTION:
        return buildFunction(root, tokens, pos);
        break;
    case BODY:
        return buildBody(root, tokens, pos);
        break;
    case RETURN:
        return buildReturn(root, tokens, pos);
        break;
    case INT_LITERAL:
        return buildIntLiteral(root, tokens, pos);
        break;
    case EXPRESSION:
        return buildExpression(root, tokens, pos);
        break;
    case UNARY_OP:
        return buildUnaryOp(root, tokens, pos);
        break;
    default:
        break;
    }

    return -1;
}

struct Ast *parse(struct Token **tokens)
{
    struct Ast *ast = malloc(sizeof(struct Ast));

    long pos = 0;
    ast->program = nodeFactory(PROGRAM, tokens[0]->pos);

    if (buildRule(ast->program, tokens, &pos))
    {
        return NULL;
    }

    return ast;
}