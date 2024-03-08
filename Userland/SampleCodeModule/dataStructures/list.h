#ifndef LST_H
#define LST_H
#include "string.h"

typedef string listElem;

typedef struct node
{
    listElem head;
    node *tail;
} node;
typedef struct list
{
    node *first;
    node *last;
    int size;
} list;

list createList();
list createSizedList(int initialSize);
void add(list l, listElem e, int index);
listElem get(list l, int index);
void push(list l, listElem e);
listElem pop(list l);
void freeList(list l);

#endif