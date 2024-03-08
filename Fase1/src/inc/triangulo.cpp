#include <vector>
#include "triangulo.hpp"

struct Ponto {
	float x;
	float y;
	float z;
};

struct triangulo {
	Ponto p1;
	Ponto p2;
	Ponto p3;
};

Triangulo novoTriangulo(Ponto p1, Ponto p2, Ponto p3) {
	Triangulo tri = (Triangulo)malloc(sizeof(struct triangulo));
	tri->p1 = p1;
	tri->p2 = p2;
	tri->p3 = p3;
	return tri;
}


void adicionarTriangulo(Figura plano, Triangulo triangulo) {
	plano.triang.push_back(triangulo);
}