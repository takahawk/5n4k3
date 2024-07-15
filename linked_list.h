#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

#include <stddef.h>

typedef struct ListNode {
	void *val;
	struct ListNode *next;
} ListNode;

typedef struct {
	size_t elemSize;
	ListNode *head;
} LinkedList;

LinkedList*
AllocLinkedList();

void
LinkedListAdd(LinkedList*, void*);

void
LinkedListRemoveHead(LinkedList*);

void
FreeLinkedList(LinkedList*);

#endif
