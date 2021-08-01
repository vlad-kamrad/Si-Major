#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

int str2int(char *string)
{
    return atoi(string);
}

char *int2str(int number)
{
    int length = snprintf(NULL, 0, "%d", number);
    char *str = malloc(length + 1);
    snprintf(str, length + 1, "%d", number);
    return str;
}

int getPairIndex(char *source, int index, char first, char second)
{
    int calc = 1;
    for (unsigned long i = index + 1; i < strlen(source); i++)
    {
        if (source[i] == first)
            calc++;

        if (source[i] == second)
            calc--;

        if (calc == 0)
            return i;
    }

    return index;
}

struct FileMetadata new_FileMetadata()
{
    struct FileMetadata instance;

    instance.size = 0;
    instance.location = NULL;

    return instance;
}
