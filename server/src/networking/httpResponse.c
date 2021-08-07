#include <stdlib.h>
#include <string.h>
#include "httpResponse.h"
#include "../file/fileObject.h"
#include "../utils/utils.h"

char *headerSeparator = ": ";
char *newLineSeparator = "\r\n";
char *bodySeparator = "\r\n\n";

struct httpResponse new_httpResponse(struct FileObject *fo)
{
    struct httpResponse instance;

    instance.firstLine = RESPONSE_200;
    instance.headers = new_Dictionary();
    instance.body = getFileObjectData(fo);

    dictAdd(&instance.headers, "Server", SERVER_NAME);
    dictAdd(&instance.headers, "Content-Type", get_MIME_Type(fo->extension));

    return instance;
}

char *build_httpResponse(struct httpResponse *instance)
{
    // TODO: Change this !!!
    // Add first line
    int approximateHeadersSize = instance->headers.count * 50;
    char *out = (char *)calloc(strlen(instance->body) + approximateHeadersSize, sizeof(char));

    strcat(out, instance->firstLine);
    strcat(out, newLineSeparator);

    // Add headers
    for (int i = 0; i < instance->headers.count; i++)
    {
        strcat(out, instance->headers.items[i].key);
        strcat(out, headerSeparator);
        strcat(out, instance->headers.items[i].value);
        strcat(out, newLineSeparator);
    }

    if (instance->body == NULL)
        return out;

    strcat(out, "\r\n");
    strcat(out, instance->body);

    return out;
}
