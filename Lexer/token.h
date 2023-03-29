#ifndef TOKEN_H
#define TOKEN_H

enum TokenType {
    OPEN_BRACE,
    CLOSE_BRACE,
    OPEN_PAREN,
    CLOSE_PAREN,
    SEMICOLON,
    INT_KEYWORD,
    RETURN_KEYWORD,
    IDENTIFIER,
    INT_LITERAL,

    EOF_T,
    ERROR_T
};

struct Token
{
    enum TokenType token_type;
};


#endif