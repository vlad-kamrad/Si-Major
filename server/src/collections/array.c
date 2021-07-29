#include <stdlib.h>
#include "array.h"

void arrayRelocateMemory(struct Array *instance, int newCountItems)
{
    instance->item = realloc(instance->item, newCountItems * sizeof(char *));
}

struct Array new_Array()
{
    struct Array instance;
    instance.count = 0;
    instance.item = malloc(0);
    return instance;
}

void free_Array(struct Array *instance);

void arrayAdd(struct Array *instance, char *value)
{
    arrayRelocateMemory(instance, instance->count + 1);
    instance->item[instance->count] = value;
    instance->count++;
}
