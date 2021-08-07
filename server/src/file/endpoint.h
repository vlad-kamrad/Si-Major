#include "fileObject.h"

#ifndef endpoint_h
#define endpoint_h

struct Endpoint
{
    char *path;
    char *endpointStr;
    int isDynamicRead;
    int fileSize;
};

struct EndpointObject
{
    char *endpoint;
    struct FileObject file;
};

struct Endpoint new_Endpoint(char *endpointStr, char *path, int isDynamicRead);
void free_Endpoint(struct Endpoint *instance);

struct EndpointObject new_EndpointObject(char *endpoint, char *location, int isDynamicLoad);

#endif /* endpoint_h */
