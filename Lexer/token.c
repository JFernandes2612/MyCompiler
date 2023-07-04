#include "token.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char *tokenToString(const struct Token *token)
{
    char *token_string = malloc(BUFF_SIZE);

    switch (token->token_type)
    {
    case OPEN_BRACE_T: // {
        strcpy(token_string, "OPEN_BRACE");
        break;
    case CLOSE_BRACE_T: // }
        strcpy(token_string, "CLOSE_BRACE");
        break;
    case OPEN_PAREN_T: // (
        strcpy(token_string, "OPEN_PAREN");
        break;
    case CLOSE_PAREN_T: // )
        strcpy(token_string, "CLOSE_PAREN");
        break;
    case SEMICOLON_T: // ;
        strcpy(token_string, "SEMICOLON");
        break;
    case INT_KEYWORD_T: // int
        strcpy(token_string, "INT_KEYWORD");
        break;
    case RETURN_KEYWORD_T: // return
        strcpy(token_string, "RETURN_KEYWORD");
        break;
    case IDENTIFIER_T: // var
        strcpy(token_string, "IDENTIFIER");
        break;
    case INT_LITERAL_T: // 123456789
        strcpy(token_string, "INT_LITERAL");
        break;
    case MINUS_T: // -
        strcpy(token_string, "MINUS");
        break;
    case PLUS_T: // +
        strcpy(token_string, "PLUS");
        break;
    case TIMES_T: // *
        strcpy(token_string, "TIMES");
        break;
    case DIV_T: // /
        strcpy(token_string, "DIV");
        break;
    case NEG_T: // ~
        strcpy(token_string, "NEG");
        break;
    case LOG_NEG_T: // !
        strcpy(token_string, "LOG_NEG");
        break;
    case EOF_T: // EOF
        strcpy(token_string, "EOF");
        break;
    case AND_T: // AND
        strcpy(token_string, "AND");
        break;
    case OR_T: // OR
        strcpy(token_string, "OR");
        break;
    case EQ_T: // EQ
        strcpy(token_string, "EQ");
        break;
    case NEQ_T: // NEQ
        strcpy(token_string, "NEQ");
        break;
    case LT_T: // LT
        strcpy(token_string, "LT");
        break;
    case LTE_T: // LTE
        strcpy(token_string, "LTE");
        break;
    case GT_T: // GT
        strcpy(token_string, "GT");
        break;
    case GTE_T: // GTE
        strcpy(token_string, "GTE");
        break;
    default:
        return "";
        break;
    }

    if (token->pos != NULL)
    {
        strcat(token_string, " ");
        char *pos_string = posToString(token->pos);
        strcat(token_string, pos_string);
        free(pos_string);
    }

    if (token->value != NULL)
    {
        strcat(token_string, " (");
        char *value_string = arbitraryValueToString(token->value);
        strcat(token_string, value_string);
        strcat(token_string, ")");
        free(value_string);
    }

    token_string = realloc(token_string, strlen(token_string) + 1);

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
    char *buff = tokenToString(token);
    printf("%s", buff);
    free(buff);
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
        freeArbitraryValue(token->value);
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