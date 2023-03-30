#include "token.h"

const char* tokenToString(const struct Token token)
{
    switch (token.token_type)
    {
    case OPEN_BRACE:
        return "OPEN_BRACE";
        break;
    case CLOSE_BRACE:
        return "CLOSE_BRACE";
        break;
    case OPEN_PAREN:
        return "OPEN_PAREN";
        break;
    case CLOSE_PAREN:
        return "CLOSE_PAREN";
        break;
    case SEMICOLON:
        return "SEMICOLON";
        break;
    case INT_KEYWORD:
        return "INT_KEYWORD";
        break;
    case RETURN_KEYWORD:
        return "RETURN_KEYWORD";
        break;
    case IDENTIFIER:
        return "IDENTIFIER";
        break;
    case INT_LITERAL:
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

void printToken(const struct Token token) {
    printf(tokenToString(token));
}

void printTokenArray(const struct Token* tokens) {
    long counter = 0;

    printf("{ ");

    while (tokens[counter].token_type != EOF_T) {
        printToken(tokens[counter]);
        printf(", ");
        counter++;
    }

    printToken(tokens[counter]);

    printf(" }\n");
}