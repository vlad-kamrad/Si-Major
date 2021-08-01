#ifndef endpoint_h
#define endpoint_h

struct Endpoint
{
    char *path;
    char *endpointStr;
    int isDynamicRead;
    int fileSize;
};

struct Endpoint new_Endpoint(char *endpointStr, char *path, int isDynamicRead);
void free_Endpoint(struct Endpoint *instance);

#endif /* endpoint_h */
