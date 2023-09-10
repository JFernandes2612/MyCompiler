#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct Token *lexTokenStateMachineDigit(const char *input, long *input_string_pos, struct Pos *screen_pos)
{
    struct Token *token = tokenFactory(INT_LITERAL_T, posCopy(screen_pos), NULL);
    char c = input[*input_string_pos];
    int *value = malloc(sizeof(int));
    (*value) = 0;
    long mult = 1;

    while (isdigit(c))
    {
        (*value) *= mult;
        (*value) += (int)(c - 48);
        mult *= 10;

        forward(input_string_pos, screen_pos, 1);
        c = input[*input_string_pos];
    }

    token->value = arbitraryValueFactory(INT, value);

    return token;
}

struct Token *lexTokenStateMachineAlphaKeyword(const char *input, long *input_string_pos, struct Pos *screen_pos, const char *target_word, const enum TokenType token_type, char *value, long *counter)
{
    struct Token *token = tokenFactory(IDENTIFIER_T, posCopy(screen_pos), NULL);

    long target_word_length = strlen(target_word);
    strcpy(value, target_word);
    (*counter) = target_word_length;

    char comparation_input[TOKEN_MAX_SIZE];
    strncpy(comparation_input, input + (*input_string_pos), target_word_length);
    comparation_input[target_word_length] = '\0';

    if (strcmp(comparation_input, target_word) == 0)
    {
        freeToken(token);
        token = tokenFactory(token_type, posCopy(screen_pos), NULL);
        forward(input_string_pos, screen_pos, target_word_length);
    }

    return token;
}

struct Token *lexTokenStateMachineAlphaIntKeyword(const char *input, long *input_string_pos, struct Pos *screen_pos, char *value, long *counter)
{
    return lexTokenStateMachineAlphaKeyword(input, input_string_pos, screen_pos, "int", INT_KEYWORD_T, value, counter);
}

struct Token *lexTokenStateMachineAlphaReturnKeyword(const char *input, long *input_string_pos, struct Pos *screen_pos, char *value, long *counter)
{
    return lexTokenStateMachineAlphaKeyword(input, input_string_pos, screen_pos, "return", RETURN_KEYWORD_T, value, counter);
}

struct Token *lexTokenStateMachineAlpha(const char *input, long *input_string_pos, struct Pos *screen_pos)
{
    struct Token *token = tokenFactory(IDENTIFIER_T, posCopy(screen_pos), NULL);
    char *value = malloc(BUFF_SIZE);
    long counter = 0;

    char c = input[*input_string_pos];
    int may_be_keyword = 1;

    while (isalpha(c) || isdigit(c) || c == '_')
    {
        value[counter] = c;
        if (may_be_keyword)
        {
            may_be_keyword = 0;
            struct Token *new_token = tokenFactory(IDENTIFIER_T, posCopy(screen_pos), NULL);
            if (c == 'i')
            {
                freeToken(new_token);
                new_token = lexTokenStateMachineAlphaIntKeyword(input, input_string_pos, screen_pos, value, &counter);
            }
            else if (c == 'r')
            {
                freeToken(new_token);
                new_token = lexTokenStateMachineAlphaReturnKeyword(input, input_string_pos, screen_pos, value, &counter);
            }

            c = input[*input_string_pos];

            if (new_token->token_type != IDENTIFIER_T && !isalpha(c) && !isdigit(c) && c != '_')
            {
                free(value);
                freeToken(token);
                return new_token;
            }

            freeToken(new_token);
            continue;
        }

        counter++;
        forward(input_string_pos, screen_pos, 1);
        c = input[*input_string_pos];
    }

    value[counter] = '\0';
    value = realloc(value, strlen(value) + 1);
    token->value = arbitraryValueFactory(STRING, value);

    return token;
}

struct Token *lexTokenStateMachineSymbol(const char *input, long *input_string_pos, struct Pos *screen_pos, const struct Symbol *symbol)
{
    struct Token *token = tokenFactory(ERROR_T, posCopy(screen_pos), NULL);
    int target_word_length = strlen(symbol->str);

    char comparation_input[TOKEN_MAX_SIZE];
    strncpy(comparation_input, input + (*input_string_pos), target_word_length);
    comparation_input[target_word_length] = '\0';

    if (strcmp(comparation_input, symbol->str) == 0)
    {
        freeToken(token);
        char *value;
        if (symbol->has_value)
        {
            value = malloc(strlen(symbol->str) + 1);
            strcpy(value, symbol->str);
        }
        token = tokenFactory(symbol->token_type, posCopy(screen_pos), symbol->has_value ? arbitraryValueFactory(STRING, value) : NULL);
        forward(input_string_pos, screen_pos, target_word_length);
    }

    return token;
}

struct Token *lexTokenStateMachineSymbols(const char *input, long *input_string_pos, struct Pos *screen_pos)
{
    struct Token *token = tokenFactory(ERROR_T, posCopy(screen_pos), NULL);

    int counter = 0;

    while (token->token_type == ERROR_T && counter != symbols_to_match_size)
    {
        struct Token *new_token = lexTokenStateMachineSymbol(input, input_string_pos, screen_pos, &symbols_to_match[counter]);
        if (new_token->token_type != ERROR_T)
        {
            freeToken(token);
            return new_token;
        }
        else
            freeToken(new_token);
        counter++;
    }

    return token;
}

struct Token *lexTokenStateMachine(const char *input, long *input_string_pos, struct Pos *screen_pos)
{
    struct Token *token = tokenFactory(ERROR_T, posCopy(screen_pos), NULL);

    char c = input[*input_string_pos];

    while (c == ' ' || c == '\n' || c == '\t')
    {
        if (c == '\n')
        {
            (*input_string_pos)++;
            posNewLine(screen_pos);
        }
        else
        {
            forward(input_string_pos, screen_pos, 1);
        }

        c = input[*input_string_pos];
    }

    if (isdigit(c))
    {
        freeToken(token);
        token = lexTokenStateMachineDigit(input, input_string_pos, screen_pos);

        // Invalidate identifiers that start with a number
        if (isalpha(input[*input_string_pos]))
        {
            freeToken(token);
            token = tokenFactory(ERROR_T, posCopy(screen_pos), NULL);
        }
    }
    else if (isalpha(c))
    {
        freeToken(token);
        token = lexTokenStateMachineAlpha(input, input_string_pos, screen_pos);
    }
    else if (c == '\0')
    {
        freeToken(token);
        token = tokenFactory(EOF_T, posCopy(screen_pos), NULL);
    }
    else
    {
        freeToken(token);
        token = lexTokenStateMachineSymbols(input, input_string_pos, screen_pos);
    }

    return token;
}

struct Token **lex(const char *input)
{
    struct Token **tokens;
    if ((tokens = malloc(sizeof(struct Token *) * TOKENS_MAX_SIZE)) == NULL)
    {
        printf("Error unable to malloc memory of array of tokens");
        return NULL;
    }

    long current_token = 0;
    long input_string_pos = 0;
    struct Pos *screen_pos = posFactory(1, 1);

    while (1)
    {
        struct Token *token = lexTokenStateMachine(input, &input_string_pos, screen_pos);
        tokens[current_token] = token;

        if (token->token_type == EOF_T)
        {
            break;
        }

        if (token->token_type == ERROR_T)
        {
            printf("Unexpected token '%c' at %s\n", input[input_string_pos], posToString(screen_pos));
            free(screen_pos);
            return NULL;
        }

        current_token++;
    }

    if ((tokens = realloc(tokens, sizeof(struct Token *) * (current_token + 1))) == NULL)
    {
        free(screen_pos);
        printf("Error unable to realloc memory of array of tokens");
        return NULL;
    }

    free(screen_pos);

    return tokens;
}