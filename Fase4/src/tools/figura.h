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

vector<float> figuraToVector(Figura f);
vector<float> figuraNormalsToVector(Figura f);
vector<float> figuraTexturesToVector(Figura f, const char* textFile);

void setFiguraVertices(Figura figura, LinkedList vertices);
void setFiguraTextura(Figura f, const char* textura);
void setFiguraDiffuse(Figura f, LinkedList diffuse);
void setFiguraAmbient(Figura f, LinkedList ambient);
void setFiguraSpecular(Figura f, LinkedList specular);
void setFiguraEmissive(Figura f, LinkedList emissive);
void setFiguraShininess(Figura f, float shininess);
void setFiguraNormais(Figura f, LinkedList normais);
void setFiguraTextCoordenadas(Figura f, LinkedList textCoordenadas);


LinkedList getFiguraVertices(Figura f);
LinkedList getFiguraNormais(Figura f);
LinkedList getFiguraTextCoordenadas(Figura f);
char* getFiguraTextura(Figura f);
LinkedList getFiguraDiffuse(Figura f);
LinkedList getFiguraAmbient(Figura f);
LinkedList getFiguraSpecular(Figura f);
LinkedList getFiguraEmissive(Figura f);
float getFiguraShininess(Figura f);

size_t getTotalVertices(Figura figura);

vector<float> getDif(Figura f);
vector<float> getAmb(Figura f);
vector<float> getSpec(Figura f);
vector<float> getEmi(Figura f);
#endif