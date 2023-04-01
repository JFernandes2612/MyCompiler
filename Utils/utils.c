#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Reads a file and returns it in a form of a string (char*)
const char *readFile(const char *path)
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

const struct ArbitraryValue *arbitraryValueFactory(const enum Type type, const void *value)
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
        return buff;
        break;
    case STRING:
        return (char *)arbitrary_value->value;
        break;
    default:
        break;
    }

    return "";
}