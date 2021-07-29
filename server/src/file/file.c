#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *errorReadFileString = "[ Error while opening the file ]\n";

char *readFile(char *fileLocation, int fileSize)
{
    FILE *file = fopen(fileLocation, "r");

    if (file == NULL)
    {
        printf("%s", errorReadFileString);
        exit(1);
    }

    char *output = (char *)calloc(fileSize, sizeof(char));

    int index = 0;
    char ch;

    while ((ch = fgetc(file)) != EOF)
    {
        output[index++] = ch;
    }

    fclose(file);
    return output;
}

int getFileSize(char *fileLocation)
{
    FILE *file = fopen(fileLocation, "r");
    if (file == NULL)
    {
        printf("%s", errorReadFileString);
        exit(1);
    }

    fseek(file, 0L, SEEK_END);
    int fileSize = ftell(file);
    fseek(file, 0L, SEEK_SET);

    fclose(file);

    return fileSize;
}
