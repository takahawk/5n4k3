#include "linked_list.h"

#include <stdlib.h>

LinkedList*
AllocLinkedList() {
	LinkedList *ll = malloc(sizeof(LinkedList));
	ll->head = NULL;
	return ll;
}

void
LinkedListAdd(LinkedList* ll, void* val) {
	ListNode *ln = malloc(sizeof(ListNode));
	ln->next = ll->head;
	ll->head = ln;
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
	ListNode *ln = ll.head;
	
	while (ln) {
		ListNode *p = ln;
		free(p);
		ln = ln->next;
	}
	free(ll);
}
