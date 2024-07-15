#include "linked_list.h"

#include <stdlib.h>
#include <string.h>

LinkedList*
AllocLinkedList(size_t elemSize) {
	LinkedList *ll = malloc(sizeof(LinkedList));
	ll->head = NULL;
	ll->elemSize = elemSize;
	return ll;
}

void
LinkedListAdd(LinkedList* ll, void* val) {
	ListNode *ln = malloc(sizeof(ListNode));
	ln->next = ll->head;
	ll->head = ln;

	ln->val = malloc(ll->elemSize);
	memcpy(ll->head->val, val, ll->elemSize);
}


void
LinkedListRemoveHead(LinkedList* ll) {
	if (!ll->head)
		return;
	ListNode *ln = ll->head;
	ll->head = ll->head->next;
	free(ln);
}

void
FreeLinkedList(LinkedList* ll) {
	ListNode *ln = ll->head;
	
	while (ln) {
		ListNode *p = ln;
		ln = ln->next;
		free(p->val);
		free(p);
	}
	free(ll);
}
