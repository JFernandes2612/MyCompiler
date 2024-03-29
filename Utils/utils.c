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
        fclose(f);
        return NULL;
    }

    long file_size;
    if ((file_size = ftell(f)) <= 0)
    {
        printf("Error file is empty or invalid.\n");
        fclose(f);
        return NULL;
    }
    // Include /0 in string
    file_size++;

    if (fseek(f, 0, SEEK_SET) != 0)
    {
        printf("Error seeking start of file.\n");
        fclose(f);
        return NULL;
    }

    char *ret;

    if ((ret = malloc(file_size)) == NULL)
    {
        printf("Error unable to malloc memory of size %ld\n", file_size);
        free(ret);
        fclose(f);
        return NULL;
    }
    ret[file_size - 1] = '\0';

    fread(ret, sizeof(char), file_size, f);

    if (fclose(f))
    {
        printf("Error unable to close file\n");
        free(ret);
        return NULL;
    }

    return ret;
}

void saveFile(const char *path, const char *dataToSave)
{
    FILE *f;
    if ((f = fopen(path, "w")) == NULL)
    {
        printf("Error opening file '%s'.\n", path);
        return;
    }

    fputs(dataToSave, f);

    fclose(f);
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

char *posToString(const struct Pos *pos)
{
    char *buff = malloc(BUFF_SIZE);

    sprintf(buff, "[line: %ld, column: %ld]", pos->line, pos->column);

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

char *arbitraryValueToString(const struct ArbitraryValue *arbitrary_value)
{
    char *buff = malloc(BUFF_SIZE);
    switch (arbitrary_value->type)
    {
    case INT:
        sprintf(buff, "%d", *((int *)arbitrary_value->value));
        break;
    case STRING:
        strcpy(buff, (char *)arbitrary_value->value);
        break;
    default:
        buff = NULL;
        break;
    }

    buff = realloc(buff, strlen(buff) + 1);

    return buff;
}

void freeArbitraryValue(struct ArbitraryValue *arbitrary_value)
{
    free(arbitrary_value->value);
    free(arbitrary_value);
}

struct StringKeyArbitraryValueMapEntry *stringKeyArbitraryValueMapEntryFactory(char *key, struct ArbitraryValue *value)
{
    struct StringKeyArbitraryValueMapEntry *ret = malloc(sizeof(struct StringKeyArbitraryValueMapEntry));
    ret->key = key;
    ret->value = value;
    return ret;
}

char *stringKeyArbitraryValueMapEntryToString(const struct StringKeyArbitraryValueMapEntry *entry)
{
    char *buff = malloc(BUFF_SIZE);

    char *value = arbitraryValueToString(entry->value);

    sprintf(buff, "%s: %s", entry->key, value);

    buff = realloc(buff, strlen(buff) + 1);

    free(value);

    return buff;
}

void freeStringKeyArbitraryValueMapEntry(struct StringKeyArbitraryValueMapEntry *entry)
{
    // The 'key' is always a static string
    // The 'value' should be freed from the Tokens since it inherits the value from there
    free(entry);
}

struct StringKeyArbitraryValueMap *stringKeyArbitraryValueMapFactory()
{
    struct StringKeyArbitraryValueMap *ret = malloc(sizeof(struct StringKeyArbitraryValueMap));
    ret->entries = NULL;
    ret->number_of_entries = 0;
    return ret;
}

void stringKeyArbitraryValueMapAddItem(struct StringKeyArbitraryValueMap *map, struct StringKeyArbitraryValueMapEntry *entry)
{
    map->number_of_entries++;
    if (map->entries == NULL)
    {
        map->entries = malloc(sizeof(struct StringKeyArbitraryValueMapEntry *));
    }
    else
    {
        map->entries = realloc(map->entries, sizeof(struct StringKeyArbitraryValueMapEntry *) * map->number_of_entries);
    }

    map->entries[map->number_of_entries - 1] = entry;
}

struct ArbitraryValue *stringKeyArbitraryValueMapGetItem(struct StringKeyArbitraryValueMap *map, const char *key)
{
    for (long i = 0; i < map->number_of_entries; i++)
    {
        if (strcmp(map->entries[i]->key, key) == 0)
        {
            return map->entries[i]->value;
        }
    }

    return NULL;
}

char *stringKeyArbitraryValueMapToString(const struct StringKeyArbitraryValueMap *map)
{
    char *buff = malloc(BUFF_SIZE);

    strcpy(buff, "{");

    for (long i = 0; i < map->number_of_entries; i++)
    {
        char *map_entry_string = stringKeyArbitraryValueMapEntryToString(map->entries[i]);
        sprintf(buff, "%s %s", buff, map_entry_string);
        free(map_entry_string);

        if (i + 1 != map->number_of_entries)
        {
            strcat(buff, " ,");
        }
    }

    strcat(buff, " }");

    return buff;
}

void freeStringKeyArbitraryValueMap(struct StringKeyArbitraryValueMap *map)
{
    for (long i = 0; i < map->number_of_entries; i++)
    {
        freeStringKeyArbitraryValueMapEntry(map->entries[i]);
    }
    free(map->entries);
    free(map);
}