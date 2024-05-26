#include "vertice.h"

struct vertice {
	float x;
	float y;
	float z;
};

Vertice newVertice(float x, float y, float z) {
	Vertice newVertice = (Vertice)malloc(sizeof(struct vertice));

	if (newVertice) {
		newVertice->x = x;
		newVertice->y = y;
		newVertice->z = z;
	}

	return newVertice;
}

Vertice newVerticeSph(float a, float b, float radius) {
	float z = radius * cos(b) * cos(a);
	float x = radius * cos(b) * sin(a);
	float y = radius * sin(b);

	return newVertice(x, y, z);
}

float getVerticeX(Vertice vertice) {
	return vertice->x;
}

void setVerticeX(Vertice vertice, float x) {
	vertice->x = x;
}

float getVerticeY(Vertice vertice) {
	return vertice->y;
}

void setVerticeY(Vertice vertice, float y) {
	vertice->y = y;
}

float getVerticeZ(Vertice vertice) {
	return vertice->z;
}

void setVerticeZ(Vertice vertice, float z) {
	vertice->z = z;
}

void printVerticesLinkedList(LinkedList vertices) {
	LinkedList current = vertices;

	// Percorre a lista e imprime os valores de cada vértice
	while (current != NULL) {
		Vertice vertice = (Vertice)getData(current);
		if (vertice != NULL) {
			printf("(%.2f, %.2f, %.2f)\n", getVerticeX(vertice), getVerticeY(vertice), getVerticeZ(vertice));
		}
		current = (LinkedList)getNext(current);
	}
}
