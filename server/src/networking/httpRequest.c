#include <string.h>
#include "httpRequest.h"

const char *methods[] = {"GET", "HEAD", "POST", "PUT", "DELETE", "CONNECT", "OPTIONS", "TRACE", "PATCH"};

enum httpMethods getHttpMethod(char *string);

struct httpRequest new_httpRequest(char *request)
{
    struct httpRequest instance;

    instance.method = getHttpMethod(request);
    instance.headers = new_Dictionary();

    return instance;
}

enum httpMethods getHttpMethod(char *methodString)
{
    for (unsigned short i = 0; i < 9; i++)
        if (!strcmp(methodString, methods[i]))
            return i;

    return GET;
}
