#include "string.h"
#include "./../nstdlib/nstdlib.h"

#define BLOCK 100

string createString()
{
    return createSizedString(0);
}
string createStringFromInput(char *input)
{
    string toRet = createSizedString(strlen(input));
    addString(toRet, input);
    return toRet;
}
string createSizedString(uint64_t initialSize)
{
    string s = malloc(sizeof(stringHeader));
    s->content = initialSize ? malloc(sizeof(char) * initialSize) : null;
    s->dim = initialSize;
    s->size = 0;
    return s;
}
void addString(string receiver, char *s)
{
    uint64_t len = strlen(s);
    if (receiver->size + len > receiver->dim)
    {
        receiver->content = realloc(receiver->content, receiver->dim, ((receiver->size + len) / BLOCK) * BLOCK + 1);
    }
    concat(receiver->content, s);
}
void addChar(string receiver, char c)
{
    char impromptuString[2] = {c, 0};
    addString(receiver, impromptuString);
}
stringIterator getStringIterator(string s)
{
    stringIterator iter = malloc(sizeof(stringIterator));
    iter->index = 0;
    iter->s = s;
    return iter;
}
char nextS(stringIterator iter)
{
    if (!hasNext(iter))
        return 0;
    return iter->s->content[iter->index++];
}
char hasNextS(stringIterator iter)
{
    return iter->index > iter->s->size;
}
void freeStringIterator(stringIterator iter)
{
    free(iter);
}
void freeString(string s)
{
    if (s != null)
    {
        if (s->content != null)
            free(s->content);
        free(s);
    }
}
uint64_t size(string s)
{
    return s->size;
}