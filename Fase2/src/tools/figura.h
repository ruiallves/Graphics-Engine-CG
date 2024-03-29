#ifndef FIGURA
#define FIGURA

#include "../tools/vertice.h"
#include "vector"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

typedef struct figura* Figura;

Figura newFigura();
Figura fileToFigura(const char* model);

std::vector<Vertice> getFiguraVertices(Figura figura);
void setFiguraVertices(Figura& figura, const std::vector<Vertice>& vertices);

#endif