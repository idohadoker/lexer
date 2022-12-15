#ifndef LIST_H
#define LIST_H
#include "task.h"
void deletetask();
void addtask();
typedef struct List
{
    Task data;
    struct List *next;
} node, *nodeptr;

#endif // LIST_H