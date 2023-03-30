#include <stdlib.h>
#include "Utils/utils.h"
#include "Lexer/lexer.h"

void printUsage(const char* ex) {
    printf("Usage:\n%s [inputFile]\n", ex);
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Wrong number of arguments!\n\n");
        printUsage(argv[0]);
        return -1;
    }

    const char *input;

    if ((input = readFile(argv[1])) == NULL)
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