#include <stdlib.h>
#include "httpResponse.h"

struct httpResponse new_httpResponse()
{
    struct httpResponse instance;

    instance.firstLine = RESPONSE_200;
    instance.headers = new_Dictionary();

    return instance;
}

char *build_httpResponse(struct httpResponse *instance)
{
    return "Aboba";
}