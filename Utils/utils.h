#ifndef UTILS_H
#define UTILS_H

#define BUFF_SIZE 1028

const char *readFile(const char *path);

struct Pos
{
    long line;
    long column;
};

const struct Pos *posFactory(const long line, const long column);

const struct Pos *posCopy(const struct Pos *pos);

void posAddLine(struct Pos *pos, const long line);

void posAddColumn(struct Pos *pos, const long column);

void posNewLine(struct Pos *pos);

const char *posToString(const struct Pos *pos);

void forward(long* pos_l, struct Pos *pos_p, const long n);

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