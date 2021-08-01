#ifndef config_wrapper_h
#define config_wrapper_h

struct AllowFileProps
{
    char *pathToFile;
    int isDynamicRead;
    char **endpoints;
};

struct ConfigFile
{
    struct AllowFileProps *allowFileProps;
    int chunkSize;
};

struct ConfigFile new_OpenConfigFile(char *pathToConfigFile, int *isSuccess);
struct ConfigFile new_ReadConfigFile(char *filedata);

struct AllowFileProps new_AllowFileProps();

#endif /* config_wrapper_h */
