#include "token.h"

const char *tokenToString(const struct Token token)
{
    switch (token.token_type)
    {
    case OPEN_BRACE_T:
        return "OPEN_BRACE";
        break;
    case CLOSE_BRACE_T:
        return "CLOSE_BRACE";
        break;
    case OPEN_PAREN_T:
        return "OPEN_PAREN";
        break;
    case CLOSE_PAREN_T:
        return "CLOSE_PAREN";
        break;
    case SEMICOLON_T:
        return "SEMICOLON";
        break;
    case INT_KEYWORD_T:
        return "INT_KEYWORD";
        break;
    case RETURN_KEYWORD_T:
        return "RETURN_KEYWORD";
        break;
    case IDENTIFIER_T:
        return "IDENTIFIER";
        break;
    case INT_LITERAL_T:
        return "INT_LITERAL";
        break;
    case EOF_T:
        return "EOF";
        break;
    case ERROR_T:
        return "ERROR";
        break;
    default:
        break;
    }
}

const struct Token tokenFactory(const enum TokenType tokenType)
{
    struct Token ret;
    ret.token_type = tokenType;
    return ret;
}

void printToken(const struct Token token)
{
    printf(tokenToString(token));
}

void printTokenArray(const struct Token *tokens)
{
    long counter = 0;

    printf("{ ");

    while (tokens[counter].token_type != EOF_T)
    {
        printToken(tokens[counter]);
        printf(", ");
        counter++;
    }

    printToken(tokens[counter]);

    printf(" }\n");
}