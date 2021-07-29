#include <stdlib.h>
#include <string.h>
#include "endpoint.h"

struct Endpoint new_Endpoint()
{
    struct Endpoint instance;

    instance.endpoints = new_Array();
    instance.isDynamicRead = 0;
    instance.path = malloc(0);

    return instance;
}

int isContainsUri(struct Endpoint *instance, char *uri)
{
    for (int i = 0; i < instance->endpoints.count; i++)
    {
        if (!strcmp(instance->endpoints.item[i], uri))
            return 1;
    }

    return 0;
}
