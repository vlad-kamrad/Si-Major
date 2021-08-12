#include <string.h>
#include <stdlib.h>
#include "httpRequest.h"

const char *methods[] = {"GET", "HEAD", "POST", "PUT", "DELETE", "CONNECT", "OPTIONS", "TRACE", "PATCH"};
const char *httpDelimiter = "\r\n";
const char *httpDoubleDelimiter = "\r\n\r\n";
const char *headerDelimiter = ": ";
const char *space = " ";

static void _parseStatusLineString(struct httpRequest *instance, char *statusLine);
static void _parseHeaders(struct httpRequest *instance, char *headers);

struct httpRequest new_httpRequest(char *request)
{
    struct httpRequest instance;
    instance.headers = new_Dictionary();

    char *delimetedPointer = strstr(request, httpDelimiter);

    int position = delimetedPointer - request;
    char *statusLine = (char *)calloc(position, sizeof(char));
    memmove(statusLine, request, position);

    delimetedPointer = delimetedPointer + strlen(httpDelimiter);

    char *afterHeadersPointer = strstr(delimetedPointer, httpDoubleDelimiter);
    int headerPosition = afterHeadersPointer - delimetedPointer;
    char *headersString = (char *)calloc(headerPosition, sizeof(char));
    memmove(headersString, delimetedPointer, headerPosition);

    _parseStatusLineString(&instance, statusLine);
    _parseHeaders(&instance, headersString);

    return instance;
}

enum httpMethods getHttpMethod(char *methodString)
{
    for (unsigned short i = 0; i < 9; i++)
        if (!strcmp(methodString, methods[i]))
            return i;

    return GET;
}

const char *getHttpMethodByEnum(enum httpMethods method)
{
    return methods[method];
}

static void _parseHeaders(struct httpRequest *instance, char *headersString)
{
    char *block = strtok(headersString, httpDelimiter);
    int delimiterSize = strlen(headerDelimiter);

    while (block != NULL)
    {
        char *key = strtok(block, headerDelimiter);
        block += strlen(key) + delimiterSize;

        char *value = block;
        block += strlen(value) + delimiterSize;
        block = strtok(block, httpDelimiter);

        dictAdd(&instance->headers, key, value);
    }
}

static void _parseStatusLineString(struct httpRequest *instance, char *statusLine)
{
    char *block = strtok(statusLine, space);
    instance->method = getHttpMethod(block);
    instance->uri = strtok(NULL, space);
    instance->httpVersion = strtok(NULL, space);
}
