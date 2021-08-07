#include "../collections/dictionary.h"

#ifndef httpResponse_h
#define httpResponse_h

#define RESPONSE_200 "HTTP/1.1 200 OK\r\n\n"
#define RESPONSE_404 "HTTP/1.1 404 Not Found\r\n\n"

struct httpResponse
{
    char *firstLine;
    struct Dictionary headers;
};

struct httpResponse new_httpResponse();

char *build_httpResponse(struct httpResponse *instance);

#endif /* httpResponse_h */
