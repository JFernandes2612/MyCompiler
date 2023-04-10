#include "visitor.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void codeGenerationVisitIntLiteral(struct Node *node, char *assemblyCode)
{
    char *int_value_in_string = arbitraryValueToString(stringKeyArbitraryValueMapGetItem(node->data, "value"));
    char *buff = malloc(BUFF_SIZE);
    strcpy(buff, "");
    sprintf(buff, "\tmovl $%s, %%eax\n", int_value_in_string);
    strcat(assemblyCode, buff);
    free(buff);
    free(int_value_in_string);
}

void codeGenerationVisitUnaryOp(struct Node *node, char *assemblyCode)
{
    codeGenerationVisit(node->children[0], assemblyCode);

    char *op = arbitraryValueToString(stringKeyArbitraryValueMapGetItem(node->data, "op"));

    if (strcmp(op, "-") == 0)
        strcat(assemblyCode, "\tnegl %eax\n");
    else if (strcmp(op, "~") == 0)
        strcat(assemblyCode, "\tnotl %eax\n");
    else if (strcmp(op, "!") == 0)
        strcat(assemblyCode, "\tcmpl $0, %eax\n\tmovl $0, %eax\n\tsete %al\n");
    else // Should never happen
        printf("Unknown operator %s\n", op);

    free(op);
}

void codeGenerationVisitBinOp(struct Node *node, char *assemblyCode)
{
    codeGenerationVisit(node->children[0], assemblyCode);
    strcat(assemblyCode, "\tpush %rax\n");

    codeGenerationVisit(node->children[1], assemblyCode);
    strcat(assemblyCode, "\tpop %rcx\n");

    char *op = arbitraryValueToString(stringKeyArbitraryValueMapGetItem(node->data, "op"));

    if (strcmp(op, "+") == 0)
        strcat(assemblyCode, "\taddl %ecx, %eax\n");
    else if (strcmp(op, "-") == 0)
        strcat(assemblyCode, "\tsubl %eax, %ecx\n\tmov %ecx, %eax\n");
    else if (strcmp(op, "*") == 0)
        strcat(assemblyCode, "\timul %ecx, %eax\n");
    else if (strcmp(op, "/") == 0)
        strcat(assemblyCode, "\txorl %edx, %edx\n\tpush %rax\n\tmovl %ecx, %eax\n\tpop %rcx\n\tidivl %ecx\n");
    else // Should never happen
        printf("Unknown operator %s\n", op);

    free(op);
}

void codeGenerationVisitReturn(struct Node *node, char *assemblyCode)
{
    // Visit child expression
    codeGenerationVisit(node->children[0], assemblyCode);

    strcat(assemblyCode, "\tret\n");
}

void codeGenerationVisitFunction(struct Node *node, char *assemblyCode)
{
    char *func_name = arbitraryValueToString(stringKeyArbitraryValueMapGetItem(node->data, "funcName"));
    sprintf(assemblyCode, "%s%s:\n", assemblyCode, func_name);
    free(func_name);

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