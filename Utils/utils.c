#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

// Reads a file and returns it in a form of a string (char*)
const char *readFile(const char *path)
{
    FILE *f;
    if ((f = fopen(path, "r")) == NULL)
    {
        printf("Error opening file '%s'.", path);
        return NULL;
    }

    if (fseek(f, 0, SEEK_END) != 0)
    {
        printf("Error seeking end of file.");
        return NULL;
    }

    long file_size;
    if ((file_size = ftell(f)) <= 0)
    {
        printf("Error file is empty or invalid.");
        return NULL;
    }
    // Include /0 in string
    file_size++;

    if (fseek(f, 0, SEEK_SET) != 0)
    {
        printf("Error seeking start of file.");
        return NULL;
    }

    char *ret;

    if ((ret = malloc(file_size)) == NULL)
    {
        printf("Error unable to malloc memory of size %ld", file_size);
        return NULL;
    }
    ret[file_size - 1] = '\0';

    fread(ret, sizeof(char), file_size, f);

    return ret;
}