#ifndef PONTO
#define PONTO
#include <stdlib.h>
#include <cmath>

typedef struct ponto* Ponto;

Ponto newEmptyPonto();

Ponto newPonto(float x, float y, float z);

Ponto newPontoSPHERE(float a, float b, float radius);

float getX(Ponto p);

float getY(Ponto p);

float getZ(Ponto p);

float distanceToOrigin(Ponto p);

float distance(Ponto p1, Ponto p2);

Ponto duplicaPonto(Ponto);

void DelPonto(Ponto p);

#endif // PONTO