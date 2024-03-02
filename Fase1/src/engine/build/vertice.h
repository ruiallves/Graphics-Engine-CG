#include <stdlib.h>
#include <math.h>

struct vertice {
	float x;
	float y;
	float z;
};

typedef struct vertice* Vertice;

Vertice newVertice(float x, float y, float z);