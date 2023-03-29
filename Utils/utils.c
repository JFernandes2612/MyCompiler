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

const char *minimizeInput(const char* input) {
    long input_size = strlen(input) + 1;

    char* ret;
    
    if ((ret = malloc(input_size)) == NULL) {
        printf("Error unable to malloc memory of size %ld\n", input_size);
        return NULL;
    }

    char c;
    long counter = 0;
    long minimize_counter = 0;

    while ((c = input[counter]) != '\0') {
        if (c != '\n' && c != '\t' && c != ' ') {
            ret[minimize_counter] = c;
            minimize_counter++;
        }
        counter++;
    }

    ret[minimize_counter] = '\0';

    if ((ret = realloc(ret, strlen(ret))) == NULL) {
        printf("Error unable to realloc memory to size %ld\n", strlen(ret));
        return NULL;
    }

    free(input);

    return ret;
}