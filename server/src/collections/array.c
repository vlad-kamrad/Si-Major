#include <stdlib.h>
#include "array.h"

static void _relocateMemory(struct Array *instance, int newCountItems);

struct Array new_Array()
{
    struct Array instance;
    instance.count = 0;
    instance.item = malloc(0);
    return instance;
}

void free_Array(struct Array *instance)
{
    free(instance->item);
    instance->count = 0;
}

void arrayAdd(struct Array *instance, char *value)
{
    _relocateMemory(instance, instance->count + 1);
    instance->item[instance->count] = value;
    instance->count++;
}

static void _relocateMemory(struct Array *instance, int newCountItems)
{
    instance->item = realloc(instance->item, newCountItems * sizeof(char *));
}
