#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

typedef struct {
	void *val;
	LinkedList *next;
} ListNode;

typedef struct {
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
