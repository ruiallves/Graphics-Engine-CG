#ifndef TRI
#define TRI
#include <stdlib.h>
#include <vector>
#include "ponto.hpp"

typedef struct triangulo* Triangulo;

struct Figura {
	std::vector<triangulo> triang;
};

Triangulo novoTriangulo(Ponto p1, Ponto p2, Ponto p3);

void adicionarTriangulo(Figura plano, Triangulo triangulo);

#endif
