#include "../collections/dictionary.h"
#include "../file/fileObject.h"

#ifndef httpResponse_h
#define httpResponse_h

/* #define RESPONSE_200 "HTTP/1.1 200 OK\r\n\n"
#define RESPONSE_404 "HTTP/1.1 404 Not Found\r\n\n" */

#define SERVER_NAME "Si-Major"

#define RESPONSE_200 "HTTP/1.1 200 OK"
#define RESPONSE_404 "HTTP/1.1 404 Not Found"

struct httpResponse
{
    char *statusLine;
    struct Dictionary headers;
    char *body;
    int bodySize;
};

struct httpResponse new_httpResponse(struct FileObject *fo);

char *build_httpResponse(struct httpResponse *instance);

#endif /* httpResponse_h */
