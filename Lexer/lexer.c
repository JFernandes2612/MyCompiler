#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>

const struct Token lexTokenStateMachine(const char *input, long *pos) {
    struct Token token;
    token.token_type = ERROR_T;

    char c = input[*pos];

    switch (c)
    {
    case '(':
        token.token_type = OPEN_PAREN;
        break;
    case ')':
        token.token_type = CLOSE_PAREN;
        break;
    case '{':
        token.token_type = OPEN_BRACE;
        break;
    case '}':
        token.token_type = CLOSE_BRACE;
        break;
    case ';':
        token.token_type = SEMICOLON;
        break;
    case '\0':
        token.token_type = EOF_T;
        break;
    default:
        token.token_type = ERROR_T;
        break;
    }

    (*pos)++;

    return token;
}

const struct Token* lex(const char* input) {
    long input_pos = 0;

    struct Token* tokens;
    if ((tokens = malloc(sizeof(struct Token) * TOKENS_MAX_SIZE)) == NULL) {
        printf("Error unable to malloc memory of array of tokens");
        return NULL;
    }
    long current_token = 0;

    while (1) {
        struct Token token = lexTokenStateMachine(input, &input_pos);
        tokens[current_token] = token;

        if (token.token_type == EOF_T) {
            break;
        }

        if (token.token_type == ERROR_T) {
            printf("Error parsing tokenizing input\n");
            return NULL;
        }

        current_token++;
    }

    if ((tokens = realloc(tokens, sizeof(struct Token) * (current_token + 1))) == NULL) {
        printf("Error unable to realloc memory of array of tokens");
        return NULL;
    }

    return tokens;
}