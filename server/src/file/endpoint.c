#include <stdlib.h>
#include <string.h>
#include "../collections/array.h"
#include "endpoint.h"

struct Endpoint new_Endpoint(char *endpointStr, char *path, int isDynamicRead)
{
    int pathSize = strlen(path);
    int endpointStrSize = strlen(endpointStr);

    struct Endpoint instance;

    instance.path = (char *)calloc(pathSize, sizeof(char));
    memmove(instance.path, path, pathSize);

    instance.endpointStr = (char *)calloc(endpointStrSize, sizeof(char));
    memmove(instance.endpointStr, endpointStr, endpointStrSize);

    instance.isDynamicRead = isDynamicRead;

    return instance;
}

void free_Endpoint(struct Endpoint *instance)
{
    free(instance->endpointStr);
    free(instance->path);
}
