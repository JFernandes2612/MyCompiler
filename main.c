#include <stdlib.h>
#include "Utils\utils.h"
#include "Lexer\lexer.h"

int main()
{
    const char *input;

    if ((input = readFile("input.txt")) == NULL)
    {
        printf("Error reading source file\n");
        return -1;
    }

    if ((input = minimizeInput(input)) == NULL)
    {
        printf("Error minimizing input\n");
        return -1;
    }

    const struct Token* tokens;

    if ((tokens = lex(input)) == NULL) {
        printf("Error tokenizing input\n");
        return -1;
    }

    return 0;
}