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

    // dictAdd(&instance.headers, "Accept-Ranges", "bytes");
    // dictAdd(&instance.headers, "Content-Transfer-Encoding", "base64");

    return instance;
}

unsigned char hex2bin(const char *s)
{
    int ret = 0;
    int i;
    for (i = 0; i < 2; i++)
    {
        char c = *s++;
        int n = 0;
        if ('0' <= c && c <= '9')
            n = c - '0';
        else if ('a' <= c && c <= 'f')
            n = 10 + c - 'a';
        else if ('A' <= c && c <= 'F')
            n = 10 + c - 'A';
        ret = n + ret * 16;
    }
    return (unsigned char)ret;
}

static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};
static char *decoding_table = NULL;
static int mod_table[] = {0, 2, 1};

void build_decoding_table()
{

    decoding_table = malloc(256);

    for (int i = 0; i < 64; i++)
        decoding_table[(unsigned char)encoding_table[i]] = i;
}

char *base64_encode(const unsigned char *data,
                    size_t input_length,
                    size_t *output_length)
{

    *output_length = 4 * ((input_length + 2) / 3);

    char *encoded_data = malloc(*output_length);
    if (encoded_data == NULL)
        return NULL;

    for (int i = 0, j = 0; i < input_length;)
    {

        uint32_t octet_a = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_b = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_c = i < input_length ? (unsigned char)data[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
    }

    for (int i = 0; i < mod_table[input_length % 3]; i++)
        encoded_data[*output_length - 1 - i] = '=';

    return encoded_data;
}

char *build_httpResponse(struct httpResponse *instance)
{
    // TODO: Change this !!!
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

    // if (!instance->bodySize)
    //     return out;

    strcat(out, "\n");
    strcat(out, instance->body);

    // size_t bbLenght = 0;
    // char *aboba = base64_encode(instance->body, instance->bodySize, &bbLenght);
    //strcat(out, "data:image/jpeg;base64,");
    //strcat(out, aboba);
    //dictAdd(&instance->headers, "Content-Length", int2str(aboba));

    return out;
}
