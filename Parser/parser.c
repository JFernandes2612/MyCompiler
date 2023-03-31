#include "parser.h"
#include <stdlib.h>

int addChildToRootAst(struct Node *root, const struct Token *tokens, long *pos, const enum NodeType node_type)
{
    struct Node functionNode = nodeFactory(node_type);
    if (buildRule(&functionNode, tokens, pos))
    {
        return -1;
    }
    addChild(root, functionNode);

    return 0;
}

int testToken(const struct Token *tokens, long *pos, enum TokenType token_type)
{
    const struct Token token = tokens[*pos];
    if (token.token_type != token_type)
    {
        printf("Expected '%s' found '%s'\n", tokenToString(tokenFactory(token_type)), tokenToString(token));
        return -1;
    }

    (*pos)++;

    return 0;
}

int testTokens(const struct Token *tokens, long *pos, const enum TokenType *token_types, const long number_of_tests)
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
    case INT_LITERAL:
        return buildIntLiteral(root, tokens, pos);
        break;
    default:
        break;
    }

    return -1;
}

int buildProgram(struct Node *root, const struct Token *tokens, long *pos)
{
    if (addChildToRootAst(root, tokens, pos, FUNCTION))
    {
        return -1;
    }

    if (testToken(tokens, pos, EOF_T))
    {
        return -1;
    }

    return 0;
}

int buildFunction(struct Node *root, const struct Token *tokens, long *pos)
{
    const enum TokenType token_to_test[5] = {INT_KEYWORD_T, IDENTIFIER_T, OPEN_PAREN_T, CLOSE_PAREN_T, OPEN_BRACE_T};

    if (testTokens(tokens, pos, token_to_test, 5))
    {
        return -1;
    }

    if (addChildToRootAst(root, tokens, pos, BODY))
    {
        return -1;
    }

    if (testToken(tokens, pos, CLOSE_BRACE_T))
    {
        return -1;
    }

    return 0;
}

int buildBody(struct Node *root, const struct Token *tokens, long *pos)
{
    if (addChildToRootAst(root, tokens, pos, RETURN))
    {
        return -1;
    }

    return 0;
}

int buildReturn(struct Node *root, const struct Token *tokens, long *pos)
{
    if (testToken(tokens, pos, RETURN_KEYWORD_T))
    {
        return -1;
    }

    // Will later change to expression for now it can only return int literals
    if (addChildToRootAst(root, tokens, pos, INT_LITERAL))
    {
        return -1;
    }

    if (testToken(tokens, pos, SEMICOLON_T))
    {
        return -1;
    }

    return 0;
}

int buildIntLiteral(struct Node *root, const struct Token *tokens, long *pos)
{
    if (testToken(tokens, pos, INT_LITERAL_T))
    {
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