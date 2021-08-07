#include "fileObject.h"
#include "file.h"
#include <stdlib.h>
#include <string.h>

static char *_getExtension(char *location);
static char *_readFileData(struct FileObject *instance);

struct FileObject new_FileObject(char *location, int isDynamicLoad)
{
    struct FileObject instance;

    instance.isDynamicLoad = isDynamicLoad;
    instance.location = location;
    instance.size = getFileSize(location);
    instance.extension = _getExtension(location);
    instance.data = !isDynamicLoad ? _readFileData(&instance) : malloc(0);

    return instance;
}

char *getFileObjectData(struct FileObject *instance)
{
    return instance->isDynamicLoad ? _readFileData(instance) : instance->data;
}

static char *_getExtension(char *location)
{
    char *ext = strrchr(location, '.');
    return ext ? ext + 1 : NULL;
}

static char *_readFileData(struct FileObject *instance)
{
    int isSuccess = 0;
    char *data = readFile(instance->location, instance->size, &isSuccess);
    return isSuccess ? data : NULL;
}
