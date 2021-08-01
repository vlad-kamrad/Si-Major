#include <stdlib.h>
#include "configWrapper.h"
#include "../file/file.h"
#include "stdio.h"
#include <string.h>

#include "utils.h"

void initAllowFileProps(struct ConfigFile *config, char *data);
void initChunkSize(struct ConfigFile *config, char *data);
char *removeUnnecessary(char *src, int srcLength);

const char *chunkSizePattern = "\"chunkSize\":";
const char *allowFilesPattern = "\"allowFiles\":";
const char *endBlockPattern = " ,}";

struct AllowFileProps new_AllowFileProps()
{
    struct AllowFileProps instance;

    instance.isDynamicRead = 0;
    instance.pathToFile = malloc(0);
    instance.endpoints = malloc(0);

    return instance;
}

struct ConfigFile new_OpenConfigFile(char *pathToConfigFile, int *isSuccess)
{
    int configFileSize = getFileSize(pathToConfigFile);
    char *output = readFile(pathToConfigFile, configFileSize, isSuccess);
    char *cleared = removeUnnecessary(output, configFileSize);

    free(output);

    return new_ReadConfigFile(cleared);
}
struct ConfigFile new_ReadConfigFile(char *filedata)
{
    struct ConfigFile config;

    initChunkSize(&config, filedata);
    initAllowFileProps(&config, filedata);

    return config;
}

void initAllowFileProps(struct ConfigFile *config, char *data)
{
    // struct AllowFileProps *allowed;
    printf("cof %d", config->chunkSize);
    char *start = strstr(data, allowFilesPattern);
    start += strlen(allowFilesPattern);

    int arraySize = getPairIndex(start, 0, '[', ']') + 1;

    char *jsonArray = (char *)calloc(arraySize, sizeof(char));
    memmove(jsonArray, start, arraySize);

    printf("%s", data);
}

void initChunkSize(struct ConfigFile *config, char *data)
{
    char *result = strstr(data, chunkSizePattern);

    result += strlen(chunkSizePattern);

    config->chunkSize = str2int(strtok(result, endBlockPattern));
}

char *removeUnnecessary(char *src, int srcLength)
{
    char item;
    int allowRemove = 1, resIndex = 0;
    char *result = (char *)calloc(srcLength, sizeof(char));

    for (int i = 0; i < srcLength; i++)
    {
        item = src[i];

        if (item == '\"')
            allowRemove = allowRemove ? 0 : 1;

        if (allowRemove && (item == '\n' || item == ' '))
            continue;

        result[resIndex++] = item;
    }

    result = (char *)realloc(result, resIndex * sizeof(char));

    return result;
}
