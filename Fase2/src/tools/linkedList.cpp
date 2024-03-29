#include "linkedList.h"
#include "figura.h"
#include "../engine/config/world_config.h"

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

void* getNext(LinkedList ll) {
	if (ll && ll->next != NULL) {
		return ll->next;
	}
	return NULL;
}

void addChild(LinkedList* ll, void* data) {
    if (ll && data) {
        LinkedList new_node = newLinkedList(data);
        if (new_node) {
            if (*ll == NULL) {
                *ll = new_node;
            } else {
                LinkedList current = *ll;
                while (current->next != NULL) {
                    current = current->next;
                }
                current->next = new_node;
            }
        }
    }
}

void addLinkedChild(LinkedList ll, LinkedList ch) {
	if (ll && ch) {
		if (ll->next == NULL) {
			ll->next = ch;
		}
		else {
			LinkedList current = ll;
			while (current->next != NULL) {
				current = current->next;
			}
			current->next = ch;
		}
	}
}

size_t getSizeOfTransformacoes(LinkedList transformacoes) {
	size_t size = 0;
	LinkedList currentNode = transformacoes;

	while (currentNode != NULL) {
		size++;
		currentNode = (LinkedList)getNext(currentNode);
	}

	return size;
}

size_t getSizeOfFiguras(LinkedList figuras) {
	size_t size = 0;
	LinkedList currentNode = figuras;

	while (currentNode != NULL) {
		size++;
		currentNode = (LinkedList)getNext(currentNode);
	}

	return size;
}

void* getListElemAt(LinkedList list, unsigned long index) {
	if (list == nullptr) {
		std::cerr << "Lista não inicializada." << std::endl;
		return nullptr;
	}

	LinkedList currentNode = list;
	unsigned long currentIndex = 0;

	// Percorre a lista até o índice desejado ou até o final da lista
	while (currentNode != nullptr && currentIndex < index) {
		currentNode = (LinkedList)getNext(currentNode);
		currentIndex++;
	}

	// Verifica se o índice fornecido está dentro do alcance da lista
	if (currentNode == nullptr) {
		std::cerr << "Índice fora do alcance da lista." << std::endl;
		return nullptr;
	}

	// Retorna o dado armazenado no nó correspondente ao índice fornecido
	return getData(currentNode);
}
