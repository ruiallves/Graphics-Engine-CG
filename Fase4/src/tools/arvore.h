#ifndef ARVORE
#define ARVORE
#include "linkedList.h"

typedef struct arvore* Arvore;

Arvore newArvoreEmpty();
Arvore newArvore(void* data);

void* getDataArvore(Arvore nodo);
void setDataArvore(Arvore nodo, void* newData);
LinkedList getFilhosArvore(Arvore nodo);
void setFilhosArvore(Arvore nodo, LinkedList newFilhos);

void addFilhoArvore(Arvore arv, void* data);
void appendArvore(Arvore arv, Arvore child);

#endif