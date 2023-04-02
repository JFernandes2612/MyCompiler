#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Reads a file and returns it in a form of a string (char*)
char *readFile(const char *path)
{
    FILE *f;
    if ((f = fopen(path, "r")) == NULL)
    {
        printf("Error opening file '%s'.\n", path);
        return NULL;
    }

    if (fseek(f, 0, SEEK_END) != 0)
    {
        printf("Error seeking end of file.\n");
        return NULL;
    }

    long file_size;
    if ((file_size = ftell(f)) <= 0)
    {
        printf("Error file is empty or invalid.\n");
        return NULL;
    }
    // Include /0 in string
    file_size++;

    if (fseek(f, 0, SEEK_SET) != 0)
    {
        printf("Error seeking start of file.\n");
        return NULL;
    }

    char *ret;

    if ((ret = malloc(file_size)) == NULL)
    {
        printf("Error unable to malloc memory of size %ld\n", file_size);
        return NULL;
    }
    ret[file_size - 1] = '\0';

    fread(ret, sizeof(char), file_size, f);

    return ret;
}

struct Pos *posFactory(const long line, const long column)
{
    struct Pos *ret = malloc(sizeof(struct Pos));
    ret->line = line;
    ret->column = column;
    return ret;
}

struct Pos *posCopy(const struct Pos *pos)
{
    struct Pos *ret = malloc(sizeof(struct Pos));
    ret->line = pos->line;
    ret->column = pos->column;
    return ret;
}

void posAddLine(struct Pos *pos, const long line)
{
    pos->line += line;
}

void posAddColumn(struct Pos *pos, const long column)
{
    pos->column += column;
}

void posNewLine(struct Pos *pos)
{
    posAddLine(pos, 1);
    pos->column = 1;
}

const char *posToString(const struct Pos *pos)
{
    char *buff = malloc(BUFF_SIZE);

    sprintf(buff, "[line: %d, column: %d]", pos->line, pos->column);

    buff = realloc(buff, strlen(buff) + 1);

    return buff;
}

void forward(long *pos_l, struct Pos *pos_p, const long n)
{
    posAddColumn(pos_p, n);
    (*pos_l) += n;
}

struct ArbitraryValue *arbitraryValueFactory(const enum Type type, void *value)
{
    struct ArbitraryValue *ret = malloc(sizeof(struct ArbitraryValue));
    ret->type = type;
    ret->value = value;

    return ret;
}

const char *arbitraryValueToString(const struct ArbitraryValue *arbitrary_value)
{
    char *buff = malloc(BUFF_SIZE);
    switch (arbitrary_value->type)
    {
    case INT:
        itoa(*((int *)arbitrary_value->value), buff, 10);
        buff = realloc(buff, strlen(buff));
        return buff;
        break;
    case STRING:
        free(buff);
        return (char *)arbitrary_value->value;
        break;
    default:
        break;
    }

    return "";
}