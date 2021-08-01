#include <stdlib.h>
#include <string.h>
#include "dictionary.h"
#include <stdio.h>

struct Dictionary new_Dictionary()
{
    struct Dictionary instance;

    instance.count = 0;
    instance.items = malloc(0);

    return instance;
}

void free_Dictionary(struct Dictionary *instance)
{
    free(instance->items);
}

void relocateMemory(struct Dictionary *instance, int newCountItems)
{
    instance->items = realloc(instance->items, newCountItems * sizeof(struct DictionaryItem));
}

void dictAddItem(struct Dictionary *instance, struct DictionaryItem item)
{
    relocateMemory(instance, instance->count + 1);
    instance->items[instance->count] = item;
    instance->count++;
}

void dictAdd(struct Dictionary *instance, char *key, char *value)
{
    struct DictionaryItem item;
    item.key = key;
    item.value = value;

    dictAddItem(instance, item);
}

char *dictGet(struct Dictionary *instance, char *key)
{
    // Linear search
    for (int i = 0; i < instance->count; i++)
    {
        if (!strcmp(instance->items[i].key, key))
            return instance->items[i].value;
    }

    return NULL;
}

void dictRemove(struct Dictionary *instance, char *key)
{
    printf("%d, %s", instance->count, key);
    // TODO: Implement this
}
