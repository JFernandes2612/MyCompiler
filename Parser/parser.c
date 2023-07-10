#include "parser.h"
#include <stdlib.h>
#include <stdio.h>

int voidRule(const struct Node *root)
{
    return root->nodeType == EXPRESSION ||
           root->nodeType == ADD_SUB_OP ||
           root->nodeType == MULT_DIV_MOD_OP ||
           root->nodeType == RELAT_OP ||
           root->nodeType == EQUALITY_OP ||
           root->nodeType == BAND_OP ||
           root->nodeType == BOR_OP ||
           root->nodeType == BXOR_OP ||
           root->nodeType == AND_OP ||
           root->nodeType == OR_OP ||
           root->nodeType == SHIFT_OP ||
           ((root->nodeType == UNARY_OP || root->nodeType == BIN_OP) && root->data->number_of_entries == 0);
}

int testRule(struct Node *root, struct Token **tokens, long *pos, const enum NodeType node_type)
{
    struct Node *child = nodeFactory(node_type, tokens[*pos]->pos);
    long init_pos = *pos;
    if (buildRule(child, tokens, pos))
    {
        (*pos) = init_pos;
        freeNode(child);
        return -1;
    }

    if (voidRule(child))
    {
        nodeAddChildrenFromChild(root, child);
        freeNodeLessChildren(child);
    }
    else
    {
        nodeAddChild(root, child);
    }

    return 0;
}

int testToken(struct Token **tokens, long *pos, const enum TokenType token_type, const int error)
{
    const struct Token *token = tokens[*pos];

    if (token->token_type != token_type)
    {
        if (error)
            printf("Expected '%s' found '%s'\n", tokenToString(tokenFactory(token_type, NULL, NULL)), tokenToString(token));
        return -1;
    }

    (*pos)++;

    return 0;
}

int testAnyRules(struct Node *root, struct Token **tokens, long *pos, const enum NodeType *node_types, const long number_of_tests)
{
    int error = -1;

    for (long i = 0; i < number_of_tests && error; i++)
    {
        error &= testRule(root, tokens, pos, node_types[i]);
    }

    return error;
}

int testTokens(struct Token **tokens, long *pos, const enum TokenType *token_types, const long number_of_tests)
{
    for (long i = 0; i < number_of_tests; i++)
    {
        if (testToken(tokens, pos, token_types[i], 1))
        {
            return -1;
        }
    }

    return 0;
}

int testAnyTokens(struct Token **tokens, long *pos, const enum TokenType *token_types, const long number_of_tests)
{
    int error = -1;

    for (long i = 0; i < number_of_tests && error; i++)
    {
        error &= testToken(tokens, pos, token_types[i], 0);
    }

    return error;
}

int buildProgram(struct Node *root, struct Token **tokens, long *pos)
{
    if (testRule(root, tokens, pos, FUNCTION))
    {
        return -1;
    }

    if (testToken(tokens, pos, EOF_T, 1))
    {
        return -1;
    }

    return 0;
}

int buildFunction(struct Node *root, struct Token **tokens, long *pos)
{
    const enum TokenType tokens_to_test_1[2] = {INT_KEYWORD_T, IDENTIFIER_T};

    if (testTokens(tokens, pos, tokens_to_test_1, 2))
    {
        return -1;
    }

    nodePutPreviousToken(root, tokens, pos, "funcName");

    const enum TokenType tokens_to_test_2[3] = {OPEN_PAREN_T, CLOSE_PAREN_T, OPEN_BRACE_T};

    if (testTokens(tokens, pos, tokens_to_test_2, 3))
    {
        return -1;
    }

    if (testRule(root, tokens, pos, BODY))
    {
        return -1;
    }

    if (testToken(tokens, pos, CLOSE_BRACE_T, 1))
    {
        return -1;
    }

    return 0;
}

int buildBody(struct Node *root, struct Token **tokens, long *pos)
{
    if (testRule(root, tokens, pos, RETURN))
    {
        return -1;
    }

    return 0;
}

int buildReturn(struct Node *root, struct Token **tokens, long *pos)
{
    if (testToken(tokens, pos, RETURN_KEYWORD_T, 1))
    {
        return -1;
    }

    if (testRule(root, tokens, pos, EXPRESSION))
    {
        return -1;
    }

    if (testToken(tokens, pos, SEMICOLON_T, 1))
    {
        return -1;
    }

    return 0;
}

int buildExpression(struct Node *root, struct Token **tokens, long *pos)
{
    if (testRule(root, tokens, pos, OR_OP))
    {
        return -1;
    }

    return 0;
}

int buildBinOp(struct Node *root, struct Token **tokens, long *pos, const enum NodeType next_priority_node_type, const enum TokenType *operands_to_test, const long number_of_tests)
{
    if (testRule(root, tokens, pos, next_priority_node_type))
    {
        return -1;
    }

    while (1)
    {
        struct Node *binop = nodeFactory(BIN_OP, tokens[*pos]->pos);

        nodeAddChild(binop, root->children[0]);

        if (testAnyTokens(tokens, pos, operands_to_test, number_of_tests))
        {
            freeNodeLessChildren(binop);
            break;
        }

        nodePutPreviousToken(binop, tokens, pos, "op");

        if (testRule(binop, tokens, pos, next_priority_node_type))
        {
            freeNodeLessChildren(binop);
            break;
        }

        root->children[0] = binop;
    }

    return 0;
}

int buildUnaryOp(struct Node *root, struct Token **tokens, long *pos)
{
    const enum TokenType tokens_to_test[3] = {MINUS_T, NEG_T, LOG_NEG_T};

    if (testAnyTokens(tokens, pos, tokens_to_test, 3))
    {
        if (testToken(tokens, pos, OPEN_PAREN_T, 0) == 0)
        {
            if (testRule(root, tokens, pos, EXPRESSION))
            {
                return -1;
            }

            if (testToken(tokens, pos, CLOSE_PAREN_T, 0))
            {
                return -1;
            }
            return 0;
        }

        if (testRule(root, tokens, pos, INT_LITERAL))
        {
            return -1;
        }
        return 0;
    }

    nodePutPreviousToken(root, tokens, pos, "op");

    if (testRule(root, tokens, pos, UNARY_OP))
    {
        return -1;
    }

    return 0;
}

int buildIntLiteral(struct Node *root, struct Token **tokens, long *pos)
{
    if (testToken(tokens, pos, INT_LITERAL_T, 1))
    {
        return -1;
    }

    nodePutPreviousToken(root, tokens, pos, "value");

    return 0;
}

int buildRule(struct Node *root, struct Token **tokens, long *pos)
{
    switch (root->nodeType)
    {
    case PROGRAM:
        return buildProgram(root, tokens, pos);
        break;
    case FUNCTION:
        return buildFunction(root, tokens, pos);
        break;
    case BODY:
        return buildBody(root, tokens, pos);
        break;
    case RETURN:
        return buildReturn(root, tokens, pos);
        break;
    case INT_LITERAL:
        return buildIntLiteral(root, tokens, pos);
        break;
    case EXPRESSION:
        return buildExpression(root, tokens, pos);
        break;
    case UNARY_OP:
        return buildUnaryOp(root, tokens, pos);
        break;
    case MULT_DIV_MOD_OP:
        const enum TokenType tokens_to_test_mult_div_mod[3] = {TIMES_T, DIV_T, MODULO_T};
        return buildBinOp(root, tokens, pos, UNARY_OP, tokens_to_test_mult_div_mod, 3);
        break;
    case ADD_SUB_OP:
        const enum TokenType tokens_to_test_add_sub[2] = {PLUS_T, MINUS_T};
        return buildBinOp(root, tokens, pos, MULT_DIV_MOD_OP, tokens_to_test_add_sub, 2);
        break;
    case SHIFT_OP:
        const enum TokenType tokens_to_test_shift[2] = {BSL_T, BSR_T};
        return buildBinOp(root, tokens, pos, ADD_SUB_OP, tokens_to_test_shift, 2);
        break;
    case RELAT_OP:
        const enum TokenType tokens_to_test_relat[4] = {GT_T, GTE_T, LT_T, LTE_T};
        return buildBinOp(root, tokens, pos, SHIFT_OP, tokens_to_test_relat, 4);
        break;
    case EQUALITY_OP:
        const enum TokenType tokens_to_test_equality[2] = {EQ_T, NEQ_T};
        return buildBinOp(root, tokens, pos, RELAT_OP, tokens_to_test_equality, 2);
        break;
    case BAND_OP:
        const enum TokenType tokens_to_test_band[1] = {BAND_T};
        return buildBinOp(root, tokens, pos, EQUALITY_OP, tokens_to_test_band, 1);
        break;
    case BXOR_OP:
        const enum TokenType tokens_to_test_bxor[1] = {BXOR_T};
        return buildBinOp(root, tokens, pos, BAND_OP, tokens_to_test_bxor, 1);
        break;
    case BOR_OP:
        const enum TokenType tokens_to_test_bor[1] = {BOR_T};
        return buildBinOp(root, tokens, pos, BXOR_OP, tokens_to_test_bor, 1);
        break;    
    case AND_OP:
        const enum TokenType tokens_to_test_and[1] = {AND_T};
        return buildBinOp(root, tokens, pos, BOR_OP, tokens_to_test_and, 1);
        break;
    case OR_OP:
        const enum TokenType tokens_to_test_or[1] = {OR_T};
        return buildBinOp(root, tokens, pos, AND_OP, tokens_to_test_or, 1);
        break;
    default:
        break;
    }

    return -1;
}

struct Ast *parse(struct Token **tokens)
{
    struct Ast *ast = malloc(sizeof(struct Ast));

    long pos = 0;
    ast->program = nodeFactory(PROGRAM, tokens[0]->pos);

    if (buildRule(ast->program, tokens, &pos))
    {
        return NULL;
    }

    return ast;
}