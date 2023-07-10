#ifndef LEXER_H
#define LEXER_H

#include "token.h"

#define TOKENS_MAX_SIZE 1024

#define TOKEN_MAX_SIZE 128

struct Token *lexTokenStateMachineDigit(const char *input, long *input_string_pos, struct Pos *screen_pos);

struct Token *lexTokenStateMachineAlphaKeyword(const char *input, long *input_string_pos, struct Pos *screen_pos, const char *target_word, const enum TokenType token_type, char *value, long *counter);

struct Token *lexTokenStateMachineAlphaIntKeyword(const char *input, long *input_string_pos, struct Pos *screen_pos, char *value, long *counter);

struct Token *lexTokenStateMachineAlphaReturnKeyword(const char *input, long *input_string_pos, struct Pos *screen_pos, char *value, long *counter);

struct Token *lexTokenStateMachineSymbol(const char *input, long *input_string_pos, struct Pos *screen_pos, const struct Symbol *symbol);

struct Token *lexTokenStateMachineSymbols(const char *input, long *input_string_pos, struct Pos *screen_pos);

struct Token *lexTokenStateMachineAlpha(const char *input, long *input_string_pos, struct Pos *screen_pos);

struct Token *lexTokenStateMachine(const char *input, long *input_string_pos, struct Pos *screen_pos);

struct Token **lex(const char *input);

#endif