#ifndef LEXER_H
#define LEXER_H

#include "token.h"

#define TOKENS_MAX_SIZE 1024

const struct Token lexTokenStateMachine(const char *input, long *pos);

const struct Token *lex(const char *input);

#endif