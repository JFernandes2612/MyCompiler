#include "visitor.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int i = 0;

char *codeGenerationLabel()
{
    char *buff = malloc(BUFF_SIZE);
    strcpy(buff, "");
    sprintf(buff, "_label_%d", i);
    i++;
    return buff;
}

int codeGenerationVisitIntLiteral(struct Node *node, char *assemblyCode, const struct SymbolTable *symbolTable)
{
    char *int_value_in_string = arbitraryValueToString(stringKeyArbitraryValueMapGetItem(node->data, "value"));
    char *buff = malloc(BUFF_SIZE);
    strcpy(buff, "");
    sprintf(buff, "\tmovl $%s, %%eax\n", int_value_in_string);
    strcat(assemblyCode, buff);
    free(buff);
    free(int_value_in_string);
    return 0;
}

int codeGenerationVisitIdentifier(struct Node *node, char *assemblyCode, const struct SymbolTable *symbolTable)
{
    char *variable_name = arbitraryValueToString(stringKeyArbitraryValueMapGetItem(node->data, "id"));
    long variable_stack_offset = symbolTableGetVariableOffset(symbolTable, variable_name);
    char *buff = malloc(BUFF_SIZE);
    strcpy(buff, "");
    sprintf(buff, "\tmov %ld(%%rbp), %%eax\n", variable_stack_offset);
    strcat(assemblyCode, buff);
    free(buff);
    free(variable_name);

    return 0;
}

int codeGenerationVisitUnaryOp(struct Node *node, char *assemblyCode, const struct SymbolTable *symbolTable)
{
    int error = codeGenerationVisit(node->children[0], assemblyCode, symbolTable);

    char *op = arbitraryValueToString(stringKeyArbitraryValueMapGetItem(node->data, "op"));

    if (strcmp(op, "-") == 0)
        strcat(assemblyCode, "\tnegl %eax\n");
    else if (strcmp(op, "~") == 0)
        strcat(assemblyCode, "\tnotl %eax\n");
    else if (strcmp(op, "!") == 0)
        strcat(assemblyCode, "\tcmpl $0, %eax\n\tmovl $0, %eax\n\tsete %al\n");
    else // Should never happen
    {
        printf("Unknown operator %s\n", op);
        return 1;
    }

    free(op);
    return error;
}

int codeGenerationVisitBinOp(struct Node *node, char *assemblyCode, const struct SymbolTable *symbolTable)
{
    char *op = arbitraryValueToString(stringKeyArbitraryValueMapGetItem(node->data, "op"));
    int error = 0;

    if (strcmp(op, "||") == 0 || strcmp(op, "&&") == 0)
    {
        char *buff = malloc(BUFF_SIZE);
        strcpy(buff, "");

        char *label1 = codeGenerationLabel();
        char *label2 = codeGenerationLabel();

        error |= codeGenerationVisit(node->children[0], assemblyCode, symbolTable);

        strcat(assemblyCode, "\tcmpl $0, %eax\n");

        if (strcmp(op, "||") == 0)
            sprintf(buff, "\tje %s\n\tmovl $1, %%eax\n", label1);
        else if (strcmp(op, "&&") == 0)
            sprintf(buff, "\tjne %s\n", label1);
        else
        {
            printf("Unknown operator %s\n", op);
            return 1;
        }

        strcat(assemblyCode, buff);

        strcpy(buff, "");
        sprintf(buff, "\tjmp %s\n%s:\n", label2, label1);
        strcat(assemblyCode, buff);

        error |= codeGenerationVisit(node->children[1], assemblyCode, symbolTable);

        strcpy(buff, "");
        sprintf(buff, "\tcmpl $0, %%eax\n\tmovl $0, %%eax\n\tsetne %%al\n%s:\n", label2);
        strcat(assemblyCode, buff);

        free(label1);
        free(label2);
    }
    else
    {
        error |= codeGenerationVisit(node->children[0], assemblyCode, symbolTable);
        strcat(assemblyCode, "\tpush %rax\n");

        error |= codeGenerationVisit(node->children[1], assemblyCode, symbolTable);
        strcat(assemblyCode, "\tpop %rcx\n");

        if (strcmp(op, "+") == 0)
            strcat(assemblyCode, "\taddl %ecx, %eax\n");
        else if (strcmp(op, "-") == 0)
            strcat(assemblyCode, "\tsubl %eax, %ecx\n\tmov %ecx, %eax\n");
        else if (strcmp(op, "*") == 0)
            strcat(assemblyCode, "\timul %ecx, %eax\n");
        else if (strcmp(op, "/") == 0)
            strcat(assemblyCode, "\txorl %edx, %edx\n\tpush %rax\n\tmovl %ecx, %eax\n\tpop %rcx\n\tidivl %ecx\n");
        else if (strcmp(op, "|") == 0)
            strcat(assemblyCode, "\torl %ecx, %eax\n");
        else if (strcmp(op, "&") == 0)
            strcat(assemblyCode, "\tandl %ecx, %eax\n");
        else if (strcmp(op, "^") == 0)
            strcat(assemblyCode, "\txorl %ecx, %eax\n");
        else if (strcmp(op, "<<") == 0)
            strcat(assemblyCode, "\txchg %ecx, %eax\n\tshll %ecx, %eax\n");
        else if (strcmp(op, ">>") == 0)
            strcat(assemblyCode, "\txchg %ecx, %eax\n\tshrl %ecx, %eax\n");
        else if (strcmp(op, "%") == 0)
            strcat(assemblyCode, "\txorl %edx, %edx\n\tpush %rax\n\tmovl %ecx, %eax\n\tpop %rcx\n\tidivl %ecx\n\tmovl %edx, %eax\n");
        else if (strcmp(op, "==") == 0 || strcmp(op, "!=") == 0 || strcmp(op, "<=") == 0 || strcmp(op, ">=") == 0 || strcmp(op, "<") == 0 || strcmp(op, ">") == 0)
        {
            strcat(assemblyCode, "\tcmpl %eax, %ecx\n\tmovl $0, %eax\n");

            if (strcmp(op, "==") == 0)
                strcat(assemblyCode, "\tsete %al\n");
            else if (strcmp(op, "!=") == 0)
                strcat(assemblyCode, "\tsetne %al\n");
            else if (strcmp(op, "<=") == 0)
                strcat(assemblyCode, "\tsetle %al\n");
            else if (strcmp(op, ">=") == 0)
                strcat(assemblyCode, "\tsetge %al\n");
            else if (strcmp(op, "<") == 0)
                strcat(assemblyCode, "\tsetl %al\n");
            else if (strcmp(op, ">") == 0)
                strcat(assemblyCode, "\tsetg %al\n");
        }
        else // Should never happen
        {
            printf("Unknown operator %s\n", op);
            return 1;
        }
    }
    free(op);
    return error;
}

int codeGenerationVisitReturn(struct Node *node, char *assemblyCode, const struct SymbolTable *symbolTable)
{
    // Visit child expression
    int error = codeGenerationVisit(node->children[0], assemblyCode, symbolTable);

    strcat(assemblyCode, "\tmov %rbp, %rsp\n");
    strcat(assemblyCode, "\tpop %rbp\n");

    strcat(assemblyCode, "\tret\n");
    return error;
}

int codeGenerationVisitFunction(struct Node *node, char *assemblyCode, const struct SymbolTable *symbolTable)
{
    char *func_name = arbitraryValueToString(stringKeyArbitraryValueMapGetItem(node->data, "funcName"));
    sprintf(assemblyCode, "%s%s:\n", assemblyCode, func_name);
    free(func_name);

    strcat(assemblyCode, "\tpush %rbp\n");
    strcat(assemblyCode, "\tmov %rsp, %rbp\n");

    // Visit Function Body
    return codeGenerationVisit(node->children[0], assemblyCode, symbolTable);
}

int codeGenerationVisitAttribution(struct Node *node, char *assemblyCode, const struct SymbolTable *symbolTable)
{
    int error = codeGenerationVisit(node->children[0], assemblyCode, symbolTable);

    char *variable_name = arbitraryValueToString(stringKeyArbitraryValueMapGetItem(node->data, "id"));
    long variable_stack_offset = symbolTableGetVariableOffset(symbolTable, variable_name);
    char *buff = malloc(BUFF_SIZE);
    strcpy(buff, "");
    sprintf(buff, "\tmov %%eax, %ld(%%rbp)\n", variable_stack_offset);
    strcat(assemblyCode, buff);
    free(buff);
    free(variable_name);

    return 0;
}

int codeGenerationVisitDeclaration(struct Node *node, char *assemblyCode, const struct SymbolTable *symbolTable)
{
    if (node->number_of_children == 1)
    {
        return codeGenerationVisitAttribution(node, assemblyCode, symbolTable);
    }

    return 0;
}

int codeGenerationVisitProgram(struct Node *node, char *assemblyCode, const struct SymbolTable *symbolTable)
{
    strcat(assemblyCode, "\t.globl main\n");

    // Visit Function
    return codeGenerationVisit(node->children[0], assemblyCode, symbolTable);
}

int codeGenerationVisitDown(struct Node *node, char *assemblyCode, const struct SymbolTable *symbolTable)
{
    int error = 0;
    if (node->number_of_children != 0)
    {
        for (long i = 0; i < node->number_of_children; i++)
        {
            error |= codeGenerationVisit(node->children[i], assemblyCode, symbolTable);
        }
    }
    return error;
}

int codeGenerationVisit(struct Node *node, char *assemblyCode, const struct SymbolTable *symbolTable)
{
    switch (node->nodeType)
    {
    case PROGRAM:
        return codeGenerationVisitProgram(node, assemblyCode, symbolTable);
        break;
    case FUNCTION:
        return codeGenerationVisitFunction(node, assemblyCode, symbolTable);
        break;
    case DECLARATION:
        return codeGenerationVisitDeclaration(node, assemblyCode, symbolTable);
        break;
    case ATTRIBUTION:
        return codeGenerationVisitAttribution(node, assemblyCode, symbolTable);
        break;
    case RETURN:
        return codeGenerationVisitReturn(node, assemblyCode, symbolTable);
        break;
    case INT_LITERAL:
        return codeGenerationVisitIntLiteral(node, assemblyCode, symbolTable);
        break;
    case UNARY_OP:
        return codeGenerationVisitUnaryOp(node, assemblyCode, symbolTable);
        break;
    case BIN_OP:
        return codeGenerationVisitBinOp(node, assemblyCode, symbolTable);
        break;
    case IDENTIFIER:
        return codeGenerationVisitIdentifier(node, assemblyCode, symbolTable);
        break;
    default:
        return codeGenerationVisitDown(node, assemblyCode, symbolTable);
        break;
    }

    return 0;
}

char *codeGeneration(struct Ast *ast, const struct SymbolTable *symbolTable)
{
    char *assemblyCode = malloc(BUFF_SIZE);
    strcpy(assemblyCode, "");

    if (codeGenerationVisit(ast->program, assemblyCode, symbolTable))
    {
        free(assemblyCode);
        return NULL;
    }

    return assemblyCode;
}