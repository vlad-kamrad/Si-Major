#include "../collections/dictionary.h"

#ifndef file_h
#define file_h

int getFileSize(char *fileLocation);
char *readFile(char *fileLocation, int fileSize, int *isSuccess);

#endif /* file_h */
