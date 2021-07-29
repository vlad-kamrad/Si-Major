#include <string.h>
#include <stdlib.h>
#include "httpRequest.h"

const char *methods[] = {"GET", "HEAD", "POST", "PUT", "DELETE", "CONNECT", "OPTIONS", "TRACE", "PATCH"};
const char *httpDelimiter = "\r\n";
const char *httpDoubleDelimiter = "\r\n\r\n";
const char *headerDelimiter = ": ";
const char *space = " ";

enum httpMethods getHttpMethod(char *string);
void parseFirstString(struct httpRequest *instance, char *firstString);
void parseHeaders(struct httpRequest *instance, char *headers);

struct httpRequest new_httpRequest(char *request)
{
    struct httpRequest instance;
    instance.headers = new_Dictionary();

    char *__ = "POST /abobus HTTP/1.1\r\nAuthorization: Bearer token_\r\nContent-Type: application/json\r\nUser-Agent: PostmanRuntime/7.26.8\r\nAccept: */*\r\nPostman-Token: c4daa1d9-1bbc-4354-82ea-d888d6f971f0\r\nHost: 127.0.0.1:8003\r\nAccept-Encoding: gzip, deflate, br\r\nConnection: keep-alive\r\nContent-Length: 27\r\n\r\n{\n    \"nickname\": \"admin\"\n}";
    char *___ = "GET / HTTP/1.1\r\nHost: 127.0.0.1:8003\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\nsec-ch-ua: \"Chromium\";v=\"92\", \" Not A;Brand\";v=\"99\", \"Google Chrome\";v=\"92\"\r\nsec-ch-ua-mobile: ?0\r\nDNT: 1\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/92.0.4515.107 Safari/537.36\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\nSec-Fetch-Site: none\r\nSec-Fetch-Mode: navigate\r\nSec-Fetch-User: ?1\r\nSec-Fetch-Dest: document\r\nAccept-Encoding: gzip, deflate, br\r\nAccept-Language: ru-RU,ru;q=0.9,en-US;q=0.8,en;q=0.7,uk;q=0.6\r\n\r\n";

    char *delimetedPointer = strstr(request, httpDelimiter);

    int position = delimetedPointer - request;
    char *firstString = (char *)calloc(position, sizeof(char));
    memmove(firstString, request, position);

    delimetedPointer = delimetedPointer + strlen(httpDelimiter);

    char *afterHeadersPointer = strstr(delimetedPointer, httpDoubleDelimiter);
    int headerPosition = afterHeadersPointer - delimetedPointer;
    char *headersString = (char *)calloc(headerPosition, sizeof(char));
    memmove(headersString, delimetedPointer, headerPosition);

    parseFirstString(&instance, firstString);
    parseHeaders(&instance, headersString);

    return instance;
}

void parseHeaders(struct httpRequest *instance, char *headersString)
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

void parseFirstString(struct httpRequest *instance, char *firstString)
{
    char *block = strtok(firstString, space);
    instance->method = getHttpMethod(block);
    instance->uri = strtok(NULL, space);
    instance->httpVersion = strtok(NULL, space);
}

enum httpMethods getHttpMethod(char *methodString)
{
    for (unsigned short i = 0; i < 9; i++)
        if (!strcmp(methodString, methods[i]))
            return i;

    return GET;
}
