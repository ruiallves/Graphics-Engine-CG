#include "vertice.h"

Vertice newVertice(float x, float y, float z) {
	Vertice newVertice = (Vertice)malloc(sizeof(struct vertice));

	if (newVertice) {
		newVertice->x = x;
		newVertice->y = y;
		newVertice->z = z;
	}

	return newVertice;
}