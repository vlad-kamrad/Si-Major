#include "../collections/dictionary.h"

#ifndef fileObject_h
#define fileObject_h

struct FileObject
{
    int size;
    int isDynamicLoad;
    char *extension;
    char *location;
    char *data;
};

struct FileObject new_FileObject(char *location, int isDynamicLoad);
char *getFileObjectData(struct FileObject *instance);

#endif /* fileObject_h */
