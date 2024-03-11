#include "list.h"
#include "./../nstdlib/nstdlib.h"

#define BLOCK 100

list createList()
{
    list l = malloc(sizeof(listHeader));
    l->first = null;
    l->last = null;
    l->size = 0;
    return l;
}
void add(list l, listElem e)
{
    node *new = malloc(sizeof(node));
    if (l->first == null)
        l->first = new;
    l->last->tail = new;
    l->last = new;
    l->size++;
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
        node *toFree = n->tail;
        n->tail = n->tail->tail;
        free(toFree);
        return toRet;
    }
    return removeNode(n->tail, index - 1);
}
listElem remove(list l, int index)
{
    if (l == null || index >= size)
        return null;
    l->size--;
    if (l->first == l->last)
    {
        listElem toRet = l->first->head;
        l->first = null;
        l->last = null;
        return toRet;
    }
    return removeNode(l->first, index);
}
void push(list l, listElem e)
{
    node *new = malloc(sizeof(node));
    if (l->last == null)
        l->last = new;
    new->tail = l->first;
    l->first = new;
    l->size++;
}
listElem pop(list l)
{
    if (l->first != null)
    {
        l->size--;
        listElem toRet = l->first->head;
        node *toFree = l->first;
        l->first = l->first->tail;
        free(toFree);
        if (l->first == null)
            l->last = l->first;
    }
    return null;
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
listIterator getListIterator(list l)
{
    listIterator iter = malloc(sizeof(listIterator));
    iter->index = 0;
    iter->current = l->first;
    return iter;
}
listElem nextL(listIterator iter)
{
    if (!hasNext(iter))
        return 0;
    listElem toRet = iter->current->head;
    iter->current = iter->current->tail;
    return toRet;
}
char hasNextL(listIterator iter)
{
    return iter->current != null;
}
void freeListIterator(listIterator iter)
{
    free(iter);
}