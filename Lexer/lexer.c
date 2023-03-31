#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

const struct Token lexTokenStateMachineDigit(const char *input, long *pos)
{
    char c = input[*pos];

    while (isdigit(c))
    {
        (*pos)++;
        c = input[*pos];
    }

    return tokenFactory(INT_LITERAL_T);
}

const struct Token lexTokenStateMachineAlphaKeyword(const char *input, long *pos, const char *target_word, const enum TokenType token_type)
{
    struct Token token = tokenFactory(IDENTIFIER_T);

    long target_word_length = strlen(target_word);

    char comparation_input[128];
    strncpy(comparation_input, input + (*pos), target_word_length);

    if (strcmp(comparation_input, target_word) == 0)
    {
        token = tokenFactory(token_type);
        (*pos) += target_word_length;
    }

    return token;
}

const struct Token lexTokenStateMachineAlphaIntKeyword(const char *input, long *pos)
{
    return lexTokenStateMachineAlphaKeyword(input, pos, "int", INT_KEYWORD_T);
}

const struct Token lexTokenStateMachineAlphaReturnKeyword(const char *input, long *pos)
{
    return lexTokenStateMachineAlphaKeyword(input, pos, "return", RETURN_KEYWORD_T);
}

const struct Token lexTokenStateMachineAlpha(const char *input, long *pos)
{
    struct Token token = tokenFactory(IDENTIFIER_T);

    char c = input[*pos];
    int first_time = 1;

    while (isalpha(c) || isdigit(c) || c == "_")
    {
        if (first_time)
        {
            first_time = 0;
            struct Token new_token = tokenFactory(IDENTIFIER_T);
            if (c == 'i')
            {
                new_token = lexTokenStateMachineAlphaIntKeyword(input, pos);
            }
            else if (c == 'r')
            {
                new_token = lexTokenStateMachineAlphaReturnKeyword(input, pos);
            }

            c = input[*pos];

            if (new_token.token_type != IDENTIFIER_T && !isalpha(c) && !isdigit(c) && c != "_")
            {
                return new_token;
            }
            continue;
        }

        (*pos)++;
        c = input[*pos];
    }

    return token;
}

const struct Token lexTokenStateMachine(const char *input, long *pos)
{
    struct Token token = tokenFactory(ERROR_T);

    char c = input[*pos];

    while (c == ' ' || c == '\n' || c == "\t")
    {
        (*pos)++;
        c = input[*pos];
    }

    if (isdigit(c))
    {
        token = lexTokenStateMachineDigit(input, pos);

        // Invalidate identifiers that start with a number
        if (isalpha(input[*pos]))
        {
            token = tokenFactory(ERROR_T);
        }
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
            token = tokenFactory(OPEN_PAREN_T);
            break;
        case ')':
            token = tokenFactory(CLOSE_PAREN_T);
            break;
        case '{':
            token = tokenFactory(OPEN_BRACE_T);
            break;
        case '}':
            token = tokenFactory(CLOSE_BRACE_T);
            break;
        case ';':
            token = tokenFactory(SEMICOLON_T);
            break;
        case '\0':
            token = tokenFactory(EOF_T);
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