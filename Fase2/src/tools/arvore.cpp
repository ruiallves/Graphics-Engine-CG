#include "arvore.h"
#include "vector"
#include <stdlib.h>
#include <string.h>

struct arvore {
	void* data;
	LinkedList filhos;
};

Arvore newArvoreEmpty() {
	Arvore res = (Arvore)malloc(sizeof(struct arvore));
	if (res) {
		res->filhos = newLinkedListEmpty();
		res->data = NULL;
	}

	return res;
}

Arvore newArvore(void* data) {
	Arvore res = (Arvore)malloc(sizeof(struct arvore));
	if (res) {
		res->data = data;
		res->filhos = newLinkedListEmpty();
	}

	return res;
}

void* getDataArvore(Arvore nodo) {
	return nodo->data;
}

void setDataArvore(Arvore nodo, void* newData) {
	nodo->data = newData;
}

LinkedList getFilhosArvore(Arvore nodo) {
	return nodo->filhos;
}

void setFilhosArvore(Arvore nodo, LinkedList newFilhos) {
	nodo->filhos = newFilhos;
}

void addFilhoArvore(Arvore arv, void* data) {
	if (arv && data) {
		Arvore newArv = newArvore(data);
		if (newArv) {
			if (!arv->filhos) {
				arv->filhos = newLinkedListEmpty();
				if (!arv->filhos) {
					return;
				}
			}
		}
		addChild(&arv->filhos, newArv);
	}
}

void appendArvore(Arvore arv, Arvore child) {
	if (arv && child) {
		if (!arv->filhos) {
			arv->filhos = newLinkedListEmpty();
			if (!arv->filhos) {
				return;
			}
		}
		addChild(&arv->filhos, child);
	}
}