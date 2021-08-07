#include <stdlib.h>
#include <string.h>
#include "../collections/array.h"
#include "endpoint.h"
#include "fileObject.h"

struct EndpointObject new_EndpointObject(char *endpoint, char *location, int isDynamicLoad)
{
    struct EndpointObject instance;

    instance.endpoint = endpoint;
    instance.file = new_FileObject(location, isDynamicLoad);

    return instance;
}
