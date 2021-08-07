#ifndef utils_h
#define utils_h

int str2int(char *string);
char *int2str(int number);
int getPairIndex(char *source, int index, char first, char second);

struct FileMetadata
{
    int size;
    char *location;
};

struct FileMetadata new_FileMetadata();

char *get_MIME_Type(char *extension);

#endif /* utils_h */
