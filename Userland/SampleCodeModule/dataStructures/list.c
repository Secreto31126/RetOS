#include "list.h"
#include "./../nstdlib/nstdlib.h"

#define BLOCK 100

list createList()
{
    list l = malloc(sizeof(listHeader));
    l->first = null;
    l->last = null;
    return l;
}
void add(list l, listElem e)
{
    node *new = malloc(sizeof(node));
    if (l->first == null)
        l->first = new;
    l->last->tail = new;
    l->last = new;
}
listElem getNode(node *n, int index)
{
    if (n == null)
        return null;
    if (index == 0)
        return n->head;
    return getNode(n->tail, index - 1);
}
listElem get(list l, int index)
{
    if (l == null)
        return null;
    return getNode(l->first, index);
}
listElem removeNode(node *n, int index)
{
    if (n == null)
        return null;
    if (index == 1)
    {
        if (n->tail == null)
            return null;
        listElem toRet = n->tail->head;
        n->tail = n->tail->tail;
        return toRet;
    }
    return removeNode(n->tail, index - 1);
}
listElem remove(list l, int index)
{
    if (l == null)
        return null;
    return removeNode(l->first, index);
}
void push(list l, listElem e)
{
    node *new = malloc(sizeof(node));
    if (l->last == null)
        l->last = new;
    new->tail = l->first;
    l->first = new;
}
listElem pop(list l)
{
    if (l->first != null)
    {

        l->first = l->first->tail;
    }
}
void freeNode(node *n)
{
    if (n != null)
    {
        freeList(n->tail);
        free(n);
    }
}
void freeList(list l)
{
    freeNode(l->first);
    free(l);
}
