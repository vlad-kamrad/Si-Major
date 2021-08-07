#include "fileObject.h"

#ifndef endpoint_h
#define endpoint_h

struct EndpointObject
{
    char *endpoint;
    struct FileObject file;
};

struct EndpointObject new_EndpointObject(char *endpoint, char *location, int isDynamicLoad);

#endif /* endpoint_h */
