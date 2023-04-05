#include "token.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

const char *tokenToString(const struct Token *token)
{
    char *token_string = malloc(BUFF_SIZE);

    switch (token->token_type)
    {
    case OPEN_BRACE_T:
        strcpy(token_string, "OPEN_BRACE");
        break;
    case CLOSE_BRACE_T:
        strcpy(token_string, "CLOSE_BRACE");
        break;
    case OPEN_PAREN_T:
        strcpy(token_string, "OPEN_PAREN");
        break;
    case CLOSE_PAREN_T:
        strcpy(token_string, "CLOSE_PAREN");
        break;
    case SEMICOLON_T:
        strcpy(token_string, "SEMICOLON");
        break;
    case INT_KEYWORD_T:
        strcpy(token_string, "INT_KEYWORD");
        break;
    case RETURN_KEYWORD_T:
        strcpy(token_string, "RETURN_KEYWORD");
        break;
    case IDENTIFIER_T:
        strcpy(token_string, "IDENTIFIER");
        break;
    case INT_LITERAL_T:
        strcpy(token_string, "INT_LITERAL");
        break;
    case MINUS_T:
        strcpy(token_string, "MINUS");
        break;
    case NEG_T:
        strcpy(token_string, "NEG");
        break;
    case LOG_NEG_T:
        strcpy(token_string, "LOG_NEG");
        break;
    case EOF_T:
        strcpy(token_string, "EOF");
        break;
    default:
        return "";
        break;
    }

    if (token->pos != NULL)
    {
        sprintf(token_string, "%s %s", token_string, posToString(token->pos));
    }

    if (token->value != NULL)
    {
        sprintf(token_string, "%s (%s)", token_string, arbitraryValueToString(token->value));
    }

    return token_string;
}

struct Token *tokenFactory(const enum TokenType token_type, struct Pos *pos, struct ArbitraryValue *arbitrary_value)
{
    struct Token *ret = malloc(sizeof(struct Token));
    ret->token_type = token_type;
    ret->pos = pos;
    ret->value = arbitrary_value;
    return ret;
}

void printToken(const struct Token *token)
{
    printf(tokenToString(token));
}

void printTokenArray(struct Token **tokens)
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

void freeToken(struct Token *token)
{
    if (token->value != NULL)
    {
        free(token->value);
    }

    if (token->pos != NULL)
    {
        free(token->pos);
    }

    free(token);
}

void freeTokens(struct Token **tokens)
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