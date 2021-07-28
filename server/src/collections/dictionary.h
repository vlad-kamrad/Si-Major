#ifndef dictionary_h
#define dictionary_h

struct DictionaryItem
{
    char *key;
    char *value;
};

struct Dictionary
{
    int count;
    struct DictionaryItem *items;
};

struct Dictionary new_Dictionary();
void free_Dictionary(struct Dictionary *instance);

void dictAddItem(struct Dictionary *instance, struct DictionaryItem item);
void dictAdd(struct Dictionary *instance, char *key, char *value);
char *dictGet(struct Dictionary *instance, char *key);
void dictRemove(struct Dictionary *instance, char *key);

#endif /* dictionary_h */
