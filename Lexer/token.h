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
    AND_T,
    OR_T,
    EQ_T,
    NEQ_T,
    LT_T,
    LTE_T,
    GT_T,
    GTE_T,
    BOR_T,
    BAND_T,
    BXOR_T,
    BSL_T,
    BSR_T,
    MODULO_T,

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

char *tokenToString(const struct Token *token);

void printToken(const struct Token *token);

void printTokenArray(struct Token **tokens);

void freeToken(struct Token *token);

void freeTokens(struct Token **tokens);

void freeTokensWithCounter(struct Token **tokens, const int counter);

struct Symbol
{
    const char *str;
    enum TokenType token_type;
    int has_value;
};

static const int symbols_to_match_size = 25;
static const struct Symbol const symbols_to_match[25] =
    {{"==", EQ_T, 1},
     {"!=", NEQ_T, 1},
     {"<=", LTE_T, 1},
     {">=", GTE_T, 1},
     {"&&", AND_T, 1},
     {"||", OR_T, 1},
     {"<<", BSL_T, 1},
     {">>", BSR_T, 1},
     {"<", LT_T, 1},
     {">", GT_T, 1},
     {"~", NEG_T, 1},
     {"!", LOG_NEG_T, 1},
     {"+", PLUS_T, 1},
     {"-", MINUS_T, 1},
     {"*", TIMES_T, 1},
     {"/", DIV_T, 1},
     {";", SEMICOLON_T, 0},
     {"(", OPEN_PAREN_T, 0},
     {")", CLOSE_PAREN_T, 0},
     {"{", OPEN_BRACE_T, 0},
     {"}", CLOSE_BRACE_T, 0},
     {"&", BAND_T, 1},
     {"|", BOR_T, 1},
     {"^", BXOR_T, 1},
     {"%", MODULO_T, 1}};


#endif