#ifndef array_h
#define array_h

struct Array
{
    int count;
    char **item;
};

struct Array new_Array();
void free_Array(struct Array *instance);

void arrayAdd(struct Array *instance, char *value);

#endif /* array_h */
