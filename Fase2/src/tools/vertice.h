#ifndef VERTICE_H
#define VERTICE_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "linkedList.h"


typedef struct vertice* Vertice;

Vertice newVertice(float x, float y, float z);

float getVerticeX(Vertice vertice);
void setVerticeX(Vertice vertice, float x);
float getVerticeY(Vertice vertice);
void setVerticeY(Vertice vertice, float y);
float getVerticeZ(Vertice vertice);
void setVerticeZ(Vertice vertice, float z);

void printVerticesLinkedList(LinkedList vertices);

#endif /* VERTICE_H */
