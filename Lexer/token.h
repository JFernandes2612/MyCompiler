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
    MINUS_T,
    PLUS_T,
    TIMES_T,
    DIV_T,
    NEG_T,
    LOG_NEG_T,

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

struct Token *tokenFactory(const enum TokenType token_type, struct Pos *pos, struct ArbitraryValue *arbitrary_value);

const char *tokenToString(const struct Token *token);

void printToken(const struct Token *token);

void printTokenArray(struct Token **tokens);

void freeToken(struct Token *token);

void freeTokens(struct Token **tokens);

#endif