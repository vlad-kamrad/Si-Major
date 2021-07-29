#include "../collections/array.h"

#ifndef endpoint_h
#define endpoint_h

struct Endpoint
{
    char *path;
    int isDynamicRead;
    struct Array endpoints;
};

struct Endpoint new_Endpoint();
void free_Endpoint(struct Dictionary *instance);
int isContainsUri(char *uri);

#endif /* endpoint_h */
