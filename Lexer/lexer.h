#ifndef LEXER_H
#define LEXER_H

#include "token.h"

#define TOKENS_MAX_SIZE 1024

const struct Token lexTokenStateMachineDigit(const char *input, long *pos);

const struct Token lexTokenStateMachineAlphaKeyword(const char *input, long *pos, const char *target_word, const enum TokenType token_type);

const struct Token lexTokenStateMachineAlphaIntKeyword(const char *input, long *pos);

const struct Token lexTokenStateMachineAlphaReturnKeyword(const char *input, long *pos);

const struct Token lexTokenStateMachineAlpha(const char *input, long *pos);

const struct Token lexTokenStateMachine(const char *input, long *pos);

const struct Token *lex(const char *input);

#endif