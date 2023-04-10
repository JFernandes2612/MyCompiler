#ifndef UTILS_H
#define UTILS_H

#define BUFF_SIZE 1028

char *readFile(const char *path);

void saveFile(const char *path, const char *dataToSave);

struct Pos
{
    long line;
    long column;
};

struct Pos *posFactory(const long line, const long column);

struct Pos *posCopy(const struct Pos *pos);

void posAddLine(struct Pos *pos, const long line);

void posAddColumn(struct Pos *pos, const long column);

void posNewLine(struct Pos *pos);

char *posToString(const struct Pos *pos);

void forward(long *pos_l, struct Pos *pos_p, const long n);

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

struct ArbitraryValue *arbitraryValueFactory(const enum Type type, void *value);

char *arbitraryValueToString(const struct ArbitraryValue *arbitrary_value);

void freeArbitraryValue(struct ArbitraryValue *arbitrary_value);

struct StringKeyArbitraryValueMapEntry
{
    char *key;
    struct ArbitraryValue *value;
};

struct StringKeyArbitraryValueMapEntry *stringKeyArbitraryValueMapEntryFactory(char *key, struct ArbitraryValue *value);

char *stringKeyArbitraryValueMapEntryToString(const struct StringKeyArbitraryValueMapEntry *entry);

void freeStringKeyArbitraryValueMapEntry(struct StringKeyArbitraryValueMapEntry *entry);

struct StringKeyArbitraryValueMap
{
    struct StringKeyArbitraryValueMapEntry **entries;
    long number_of_entries;
};

struct StringKeyArbitraryValueMap *stringKeyArbitraryValueMapFactory();

void stringKeyArbitraryValueMapAddItem(struct StringKeyArbitraryValueMap *map, struct StringKeyArbitraryValueMapEntry *entry);

struct ArbitraryValue *stringKeyArbitraryValueMapGetItem(struct StringKeyArbitraryValueMap *map, const char *key);

char *stringKeyArbitraryValueMapToString(const struct StringKeyArbitraryValueMap *map);

void freeStringKeyArbitraryValueMap(struct StringKeyArbitraryValueMap *map);

#endif