#include "../tools/vertice.h"
#include "vector"

using namespace std;

struct figura {
	std::vector<Vertice> vertices;
};

typedef struct figura* Figura;

Figura newFigura();