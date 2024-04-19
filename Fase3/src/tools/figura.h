#ifndef FIGURA
#define FIGURA

#include "../tools/vertice.h"
#include "../tools/linkedList.h"
#include "vector"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

typedef struct figura* Figura;

Figura newFigura();
Figura fileToFigura(const char* model);

void setFiguraVertices(Figura figura, LinkedList vertices);
LinkedList getFiguraVertices(Figura f);
size_t getTotalVertices(Figura figura);

#endif