#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "../Lexer/token.h"

const struct ast parse(const struct Token *tokens);

#endif