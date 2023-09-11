#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "Utils/utils.h"
#include "Lexer/lexer.h"
#include "Parser/parser.h"
#include "CodeGeneration/visitor.h"
#include "SemanticAnalysis/analyzer.h"

void printUsage(const char *ex)
{
    printf("Usage:\n%s [inputFile]\n", ex);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Wrong number of arguments!\n\n");
        printUsage(argv[0]);
        return -1;
    }

    char *input;

    if ((input = readFile(argv[1])) == NULL)
    {
        printf("Error reading source file\n");
        return -1;
    }

    printf("Input file:\n%s\n\n", input);

    struct Token **tokens;

    if ((tokens = lex(input)) == NULL)
    {
        printf("Error tokenizing input\n");
        free(input);
        return -1;
    }

    printf("Lexer tokens:\n");
    printTokenArray(tokens);
    printf("\n\n");

    struct Ast *ast;
    if ((ast = parse(tokens)) == NULL)
    {
        printf("Error parsing input\n");
        freeTokens(tokens);
        free(input);
        return -1;
    }

    printf("AST:\n");
    printAst(ast);
    printf("\n\n");

    if (analyze(ast))
    {
        printf("Error in syntatic analysis\n");
        freeAst(ast);
        freeTokens(tokens);
        free(input);
        return -1;
    }
    printf("\n\n");

    char *assemblyCode;
    if ((assemblyCode = codeGeneration(ast)) == NULL)
    {
        printf("Error in code generation\n");
        freeAst(ast);
        freeTokens(tokens);
        free(input);
        return -1;
    }

    printf("Assembly Code:\n%s", assemblyCode);
    printf("\n\n");

    saveFile("./input.s", assemblyCode);

    int result;

#ifdef unix
    system("gcc ./input.s -o ./input.out");
    result = system("./input.out");
    result /= 256;
#elif _WIN64
    system("gcc ./input.s -o ./input.exe");
    result = system(".\\input.exe");
#endif

    printf("Result: %d\n", result);

    free(assemblyCode);
    freeAst(ast);
    freeTokens(tokens);
    free(input);

    return 0;
}