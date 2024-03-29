#ifndef LINKEDLIST
#define LINKEDLIST

#include <stdlib.h>
#include <string.h>

typedef struct linkedlist* LinkedList;

LinkedList newLinkedListEmpty();
LinkedList newLinkedList(void* data);
void setData(LinkedList ll, void* data);
void* getData(LinkedList ll);
void addChild(LinkedList* ll, void* data);

void* getNext(LinkedList ll);
void addLinkedChild(LinkedList ll, LinkedList ch);

size_t getSizeOfTransformacoes(LinkedList trans);
size_t getSizeOfFiguras(LinkedList figuras);

#endif