#ifndef MODELO
#define MODELO
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include "ponto.hpp"
#include "colecaopontos.hpp"

using namespace std;

typedef struct modelo* Modelo;

Modelo newEmptyModelo();

Modelo newModelo(colecao);

void addPonto(Modelo, Ponto);

void addPontos(Modelo, Modelo);

void ModeloToFile(Modelo, const char*);

Modelo fileToModelo(const char*);

Colecao getPontos(Modelo);

void delModelo(void*);

#endif