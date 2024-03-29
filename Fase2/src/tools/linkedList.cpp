#include "linkedList.h"
#include "figura.h"

struct linkedlist {
	void* data;
	struct linkedlist* next;
};

LinkedList newLinkedListEmpty() {
	LinkedList ll = (LinkedList)malloc(sizeof(struct linkedlist));
	if (ll) {
		ll->data = NULL;
		ll->next = NULL;
	}
	return ll;
}

LinkedList newLinkedList(void* data) {
	LinkedList ll = (LinkedList)malloc(sizeof(struct linkedlist));
	if (ll) {
		ll->data = data;
		ll->next = NULL;
	}
	return ll;
}

void setData(LinkedList ll, void* data) {
	if (ll) {
		ll->data = data;
	}
}

void* getData(LinkedList ll) {
	if (ll) {
		return ll->data;
	}
	return NULL;
}

void addChild(LinkedList* ll, void* data) {
	if (ll && data) {
		LinkedList new_node = newLinkedList(data);
		if (new_node) {
			if ((*ll)->next == NULL) {
				(*ll)->next = new_node;
			}
			else {
				LinkedList current = (*ll)->next;
				while (current->next != NULL) {
					current = current->next;
				}
				current->next = new_node;
			}
		}
	}

}


