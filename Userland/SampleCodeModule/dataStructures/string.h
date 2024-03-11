#ifndef ARR_H
#define ARR_H

typedef struct stringHeader
{
    char *content;
    int dim;
    int size;
} stringHeader;
typedef stringHeader *string;
typedef struct stringIteratorHeader
{
    string s;
    int index;
} stringIteratorHeader;
typedef stringIteratorHeader *stringIterator;

string createString();
string createSizedString(int initialSize);
void add(string receiver, char *s);
stringIterator getStringIterator(string s);
char nextS(stringIterator iter);
char hasNextS(stringIterator iter);
void freeString(string s);
void freeStringIterator(stringIterator iter);
int size(string s);

#endif