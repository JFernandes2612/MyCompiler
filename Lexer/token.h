#ifndef TOKEN_H
#define TOKEN_H

#include "../Utils/utils.h"

enum TokenType
{
    OPEN_BRACE_T,
    CLOSE_BRACE_T,
    OPEN_PAREN_T,
    CLOSE_PAREN_T,
    SEMICOLON_T,

    INT_KEYWORD_T,
    RETURN_KEYWORD_T,
    IDENTIFIER_T,

    INT_LITERAL_T,

    EOF_T,
    ERROR_T
};

struct Token
{
    enum TokenType token_type;
    struct Pos *pos;
    struct ArbitraryValue *value;
};

const struct Token *tokenFactory(const enum TokenType token_type, const struct Pos *pos, const struct ArbitraryValue *arbitrary_value);

const char *tokenToString(const struct Token *token);

void printToken(const struct Token *token);

void printTokenArray(const struct Token **tokens);

void freeToken(const struct Token *token);

void freeTokens(const struct Token **tokens);

#endif