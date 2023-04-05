#include "visitor.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void codeGenerationVisitIntLiteral(struct Node *node, char *assemblyCode)
{
    sprintf(assemblyCode, "%s\tmovl $%s, %%eax\n", assemblyCode, arbitraryValueToString(stringKeyArbitraryValueMapGetItem(node->data, "value")));
}

void codeGenerationVisitUnaryOp(struct Node *node, char *assemblyCode)
{
    codeGenerationVisit(node->children[0], assemblyCode);

    const char* op = arbitraryValueToString(stringKeyArbitraryValueMapGetItem(node->data, "op"));

    if (strcmp(op, "-") == 0)
        sprintf(assemblyCode, "%s\tnegl %%eax\n", assemblyCode);
    else if (strcmp(op, "~") == 0)
        sprintf(assemblyCode, "%s\tnotl %%eax\n", assemblyCode);
    else if (strcmp(op, "!") == 0)
        sprintf(assemblyCode, "%s\tcmpl $0, %%eax\n\tmovl $0, %%eax\n\tsete %%al\n", assemblyCode);
}

void codeGenerationVisitBinOp(struct Node *node, char *assemblyCode)
{
    codeGenerationVisit(node->children[0], assemblyCode);
    sprintf(assemblyCode, "%s\tpush %%rax\n", assemblyCode);

    codeGenerationVisit(node->children[1], assemblyCode);
    sprintf(assemblyCode, "%s\tpop %%rcx\n", assemblyCode);

    const char* op = arbitraryValueToString(stringKeyArbitraryValueMapGetItem(node->data, "op"));

    if (strcmp(op, "+") == 0)
        sprintf(assemblyCode, "%s\taddl %%ecx, %%eax\n", assemblyCode);
    else if (strcmp(op, "-") == 0)
        sprintf(assemblyCode, "%s\tsubl %%eax, %%ecx\n\tmov %%ecx, %%eax\n", assemblyCode);
    else if (strcmp(op, "*") == 0)
        sprintf(assemblyCode, "%s\timul %%ecx, %%eax\n", assemblyCode);
    else if (strcmp(op, "/") == 0)
        sprintf(assemblyCode, "%s\txorl %%edx, %%edx\n\tpush %%rax\n\tmovl %%ecx, %%eax\n\tpop %%rcx\n\tidivl %%ecx\n", assemblyCode);
    else
        printf("Uknown operator %s\n", op);
}

void codeGenerationVisitReturn(struct Node *node, char *assemblyCode)
{
    // Visit child expression
    codeGenerationVisit(node->children[0], assemblyCode);

    strcat(assemblyCode, "\tret\n");
}

void codeGenerationVisitFunction(struct Node *node, char *assemblyCode)
{
    sprintf(assemblyCode, "%s%s:\n", assemblyCode, arbitraryValueToString(stringKeyArbitraryValueMapGetItem(node->data, "funcName")));

    // Visit Function Body
    codeGenerationVisit(node->children[0], assemblyCode);
}

void codeGenerationVisitProgram(struct Node *node, char *assemblyCode)
{
    strcat(assemblyCode, "\t.globl main\n");

    // Visit Function
    codeGenerationVisit(node->children[0], assemblyCode);
}

void codeGenerationVisitDown(struct Node *node, char *assemblyCode)
{
    if (node->number_of_children != 0)
    {
        for (long i = 0; i < node->number_of_children; i++)
        {
            codeGenerationVisit(node->children[i], assemblyCode);
        }
    }
}

void codeGenerationVisit(struct Node *node, char *assemblyCode)
{
    switch (node->nodeType)
    {
    case PROGRAM:
        codeGenerationVisitProgram(node, assemblyCode);
        break;
    case FUNCTION:
        codeGenerationVisitFunction(node, assemblyCode);
        break;
    case RETURN:
        codeGenerationVisitReturn(node, assemblyCode);
        break;
    case INT_LITERAL:
        codeGenerationVisitIntLiteral(node, assemblyCode);
        break;
    case UNARY_OP:
        codeGenerationVisitUnaryOp(node, assemblyCode);
        break;
    case BIN_OP:
        codeGenerationVisitBinOp(node, assemblyCode);
        break;
    default:
        codeGenerationVisitDown(node, assemblyCode);
        break;
    }
}

char *codeGeneration(struct Ast *ast)
{
    char *assemblyCode = malloc(BUFF_SIZE);
    strcpy(assemblyCode, "");

    codeGenerationVisit(ast->program, assemblyCode);

    return assemblyCode;
}