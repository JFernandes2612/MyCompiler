#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

const struct Token lexTokenStateMachineDigit(const char *input, long *pos)
{
    struct Token token;
    token.token_type = INT_LITERAL;

    char c = input[*pos];

    while (isdigit(c))
    {
        (*pos)++;
        c = input[*pos];
    }

    return token;
}

const struct Token lexTokenStateMachineAlphaKeyword(const char *input, long *pos, const char* target_word, const enum TokenType token_type)
{
    struct Token token;
    token.token_type = IDENTIFIER;

    long target_word_length = strlen(target_word);

    char comparation_input[128];
    strncpy(comparation_input, input + (*pos), target_word_length);

    if (strcmp(comparation_input, target_word) == 0) {
        token.token_type = token_type;
        (*pos) += target_word_length;
    }

    return token;
}

const struct Token lexTokenStateMachineAlphaIntKeyword(const char *input, long *pos)
{
    return lexTokenStateMachineAlphaKeyword(input, pos, "int", INT_KEYWORD);
}

const struct Token lexTokenStateMachineAlphaReturnKeyword(const char *input, long *pos)
{
    return lexTokenStateMachineAlphaKeyword(input, pos, "return", RETURN_KEYWORD);
}

const struct Token lexTokenStateMachineAlpha(const char *input, long *pos)
{
    struct Token token;
    token.token_type = IDENTIFIER;

    char c = input[*pos];

    while (isalpha(c))
    {
        if (c == 'i') {
            token = lexTokenStateMachineAlphaIntKeyword(input, pos);
        }
        else if (c == 'r') {
            token = lexTokenStateMachineAlphaReturnKeyword(input, pos);
        }

        if (token.token_type != IDENTIFIER) {
            return token;
        }

        (*pos)++;
        c = input[*pos];
    }

    return token;
}

const struct Token lexTokenStateMachine(const char *input, long *pos)
{
    struct Token token;
    token.token_type = ERROR_T;

    char c = input[*pos];

    if (isdigit(c))
    {
        token = lexTokenStateMachineDigit(input, pos);
    }
    else if (isalpha(c))
    {
        token = lexTokenStateMachineAlpha(input, pos);
    }
    else
    {
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
        }
        (*pos)++;
    }

    return token;
}

const struct Token *lex(const char *input)
{
    long input_pos = 0;

    struct Token *tokens;
    if ((tokens = malloc(sizeof(struct Token) * TOKENS_MAX_SIZE)) == NULL)
    {
        printf("Error unable to malloc memory of array of tokens");
        return NULL;
    }
    long current_token = 0;

    while (1)
    {
        struct Token token = lexTokenStateMachine(input, &input_pos);
        tokens[current_token] = token;

        if (token.token_type == EOF_T)
        {
            break;
        }

        if (token.token_type == ERROR_T)
        {
            printf("Error parsing tokenizing input\n");
            return NULL;
        }

        current_token++;
    }

    if ((tokens = realloc(tokens, sizeof(struct Token) * (current_token + 1))) == NULL)
    {
        printf("Error unable to realloc memory of array of tokens");
        return NULL;
    }

    return tokens;
}