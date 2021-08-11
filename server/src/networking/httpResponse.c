#include <stdlib.h>
#include <string.h>
#include "httpResponse.h"
#include "../file/fileObject.h"
#include "../utils/utils.h"

#include <stdio.h>

char *headerSeparator = ": ";
char *newLineSeparator = "\r\n";
char *bodySeparator = "\r\n\n";

struct httpResponse new_httpResponse(struct FileObject *fo)
{
    struct httpResponse instance;

    instance.statusLine = RESPONSE_200;
    instance.headers = new_Dictionary();
    instance.body = getFileObjectData(fo);
    instance.bodySize = fo->size;

    dictAdd(&instance.headers, "Server", SERVER_NAME);
    dictAdd(&instance.headers, "Content-Type", get_MIME_Type(fo->extension));
    dictAdd(&instance.headers, "Content-Length", int2str(fo->size));
    dictAdd(&instance.headers, "Connection", "keep-alive");

    return instance;
}

char *build_httpResponse(struct httpResponse *instance)
{
    // Add status line
    // int approximateHeadersSize = instance->headers.count * 50;
    char *out = (char *)calloc(10000, sizeof(char));

    strcat(out, instance->statusLine);
    strcat(out, newLineSeparator);

    // Add headers
    for (int i = 0; i < instance->headers.count; i++)
    {
        strcat(out, instance->headers.items[i].key);
        strcat(out, headerSeparator);
        strcat(out, instance->headers.items[i].value);
        strcat(out, newLineSeparator);
    }

    if (instance->bodySize > 0)
        strcat(out, "\n");

    // An important point, the request body will be added directly
    // to the socket itself, this is necessary in order
    // to avoid unnecessary casts of data

    return out;
}
