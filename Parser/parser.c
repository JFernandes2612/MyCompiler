#include "parser.h"
#include <stdlib.h>

int testToken(struct Token token, enum TokenType tokenType)
{
    if (token.token_type != tokenType)
    {
        printf("Expected '%s' found '%s'\n", tokenToString(tokenFactory(tokenType)), tokenToString(token));
        return -1;
    }

    return 0;
}

int testTokens(const struct Token *tokens, long *pos, const enum TokenType* tokenType, const long number_of_tests) 
{
    for (long i = 0; i < number_of_tests; i++)
    {
        if (testToken(tokens[*pos], tokenType[i])) {
            return -1;
        }

        (*pos)++;
    }

    return 0;
}

int buildRule(struct Node *root, const struct Token *tokens, long *pos)
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
    default:
        break;
    }

    return -1;
}

int buildProgram(struct Node *root, const struct Token *tokens, long *pos)
{
    struct Node functionNode = nodeFactory(FUNCTION);
    buildRule(&functionNode, tokens, pos);
    addChild(root, functionNode);

    if (testToken(tokens[*pos], EOF_T))
    {
        return -1;
    }

    return 0;
}

int buildFunction(struct Node *root, const struct Token *tokens, long *pos)
{
    const enum TokenType token_to_test[5] = {INT_KEYWORD_T, IDENTIFIER_T, OPEN_PAREN_T, CLOSE_PAREN_T, OPEN_BRACE_T};

    if (testTokens(tokens, pos, token_to_test, 5)) {
        return -1;
    }

    struct Node bodyNode = nodeFactory(BODY);
    buildRule(&bodyNode, tokens, pos);
    addChild(root, bodyNode);

    if (testToken(tokens[*pos], CLOSE_BRACE_T))
    {
        return -1;
    }

    (*pos)++;

    return 0;
}

int buildBody(struct Node *root, const struct Token *tokens, long *pos) 
{
    struct Node returnNode = nodeFactory(RETURN);
    buildRule(&returnNode, tokens, pos);
    addChild(root, returnNode);
    return 0;
}

int buildReturn(struct Node *root, const struct Token *tokens, long *pos)
{
    const enum TokenType token_to_test[3] = {RETURN_KEYWORD_T, INT_LITERAL_T, SEMICOLON_T};

    if (testTokens(tokens, pos, token_to_test, 3)) {
        return -1;
    }

    return 0;
}

const struct Ast *parse(const struct Token *tokens)
{
    struct Ast *ast = malloc(sizeof(struct Ast));

    long pos = 0;
    ast->program = nodeFactory(PROGRAM);

    if (buildRule(&ast->program, tokens, &pos))
    {
        return NULL;
    }

    return ast;
}