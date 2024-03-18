#ifndef ARR_H
#define ARR_H

typedef struct stringHeader
{
    char *content;
    uint64_t dim;
    uint64_t size;
} stringHeader;
typedef stringHeader *string;
typedef struct stringIteratorHeader
{
    string s;
    uint64_t index;
} stringIteratorHeader;
typedef stringIteratorHeader *stringIterator;

string createString();
string createStringFromInput(char *input);
string createSizedString(uint64_t initialSize);
void addString(string receiver, char *s);
void addChar(string receiver, char c);
stringIterator getStringIterator(string s);
char nextS(stringIterator iter);
char hasNextS(stringIterator iter);
void freeString(string s);
void freeStringIterator(stringIterator iter);
uint64_t stringSize(string s);
void backspace(string receiver);

#endif