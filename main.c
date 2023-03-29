#include <stdlib.h>
#include "Utils\utils.h"

int main()
{
    const char *fileContents;

    if ((fileContents = readFile("input.txt")) == NULL)
    {
        printf("Error reading source file");
        return -1;
    }

    return 0;
}