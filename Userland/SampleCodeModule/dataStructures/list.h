#ifndef LST_H
#define LST_H
#include "string.h"

typedef string listElem;

typedef struct node
{
    listElem head;
    node *tail;
} node;
typedef struct listHeader
{
    node *first;
    node *last;
    int size;
} listHeader;
typedef listHeader *list;
typedef struct listIteratorHeader
{
    node *current;
    int index;
} listIteratorHeader;
typedef listIteratorHeader *listIterator;

list createList();
void add(list l, listElem e);
listElem get(list l, int index);
void push(list l, listElem e);
listElem pop(list l);
listIterator getListIterator(list l);
listElem nextL(listIterator iter);
char hasNextL(listIterator iter);
void freeListIterator(listIterator iter);
void freeList(list l);

#endif