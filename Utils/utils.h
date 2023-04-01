#ifndef UTILS_H
#define UTILS_H

#define BUFF_SIZE 1028

const char *readFile(const char *path);

enum Type
{
    INT,
    STRING
};

struct ArbitraryValue
{
    enum Type type;
    void *value;
};

const struct ArbitraryValue *arbitraryValueFactory(const enum Type type, const void *value);

const char *arbitraryValueToString(const struct ArbitraryValue *arbitrary_value);

struct StringKeyArbitraryValueMapEntry
{
    char *key;
    struct ArbitraryValue *value;
};

struct StringKeyArbitraryValueMap
{
    struct StringKeyArbitraryValueMapEntry *entries;
};

#endif