#include "token.h"
#include <string.h>
#include <stdio.h>

const char *tokenToString(const struct Token *token)
{
    char *token_name = malloc(BUFF_SIZE);

    switch (token->token_type)
    {
    case OPEN_BRACE_T:
        token_name = "OPEN_BRACE";
        break;
    case CLOSE_BRACE_T:
        token_name = "CLOSE_BRACE";
        break;
    case OPEN_PAREN_T:
        token_name = "OPEN_PAREN";
        break;
    case CLOSE_PAREN_T:
        token_name = "CLOSE_PAREN";
        break;
    case SEMICOLON_T:
        token_name = "SEMICOLON";
        break;
    case INT_KEYWORD_T:
        token_name = "INT_KEYWORD";
        break;
    case RETURN_KEYWORD_T:
        token_name = "RETURN_KEYWORD";
        break;
    case IDENTIFIER_T:
        token_name = "IDENTIFIER";
        break;
    case INT_LITERAL_T:
        token_name = "INT_LITERAL";
        break;
    case EOF_T:
        token_name = "EOF";
        break;
    case ERROR_T:
        token_name = "ERROR";
        break;
    default:
        return "";
        break;
    }

    if (token->value != NULL)
    {
        char *token_name_and_value = malloc(BUFF_SIZE);
        sprintf(token_name_and_value, "%s (%s)", token_name, arbitraryValueToString(token->value));
        return token_name_and_value;
    }

    return token_name;
}

const struct Token *tokenFactory(const enum TokenType token_type, const struct ArbitraryValue *arbitrary_value)
{
    struct Token *ret = malloc(sizeof(struct Token));
    ret->token_type = token_type;
    ret->value = arbitrary_value;
    return ret;
}

void printToken(const struct Token *token)
{
    printf(tokenToString(token));
}

void printTokenArray(const struct Token **tokens)
{
    long counter = 0;

    printf("{ ");

    while (tokens[counter]->token_type != EOF_T)
    {
        printToken(tokens[counter]);
        printf(", ");
        counter++;
    }

    printToken(tokens[counter]);

    printf(" }\n");
}

void freeToken(const struct Token *token)
{
    if (token->value != NULL)
    {
        free(token->value);
    }

    free(token);
}

void freeTokens(const struct Token **tokens)
{
    long counter = 0;

    while (tokens[counter]->token_type != EOF_T)
    {
        freeToken(tokens[counter]);
        counter++;
    }

    freeToken(tokens[counter]);
    free(tokens);
}