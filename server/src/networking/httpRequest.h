#ifndef httpRequest_h
#define httpRequest_h

// https://developer.mozilla.org/en-US/docs/Web/HTTP/Methods
enum httpMethods
{
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,
    PATCH
};

struct httpRequest
{
    enum httpMethods method;
    char *uri;
    char *httpVersion;
};

struct httpRequest new_httpRequest(char *request);

#endif /* httpRequest_h */
