#include "parser.h"
#include <stdlib.h>
#include <stdio.h>

int testRule(struct Node *root, struct Token **tokens, long *pos, const enum NodeType node_type)
{
    struct Node *functionNode = nodeFactory(node_type, tokens[*pos]->pos);
    if (buildRule(functionNode, tokens, pos))
    {
        return -1;
    }
    nodeAddChild(root, functionNode);

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
    default:
        break;
    }

    return -1;
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

    // Will later change to expression for now it can only return int literals
    if (testRule(root, tokens, pos, INT_LITERAL))
    {
        return -1;
    }

    if (testToken(tokens, pos, SEMICOLON_T))
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