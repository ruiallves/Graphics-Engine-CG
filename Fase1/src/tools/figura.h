#include "../tools/vertice.h"

using namespace std;

struct figura {
	std::vector<Vertice> vertices;
	char* nome;
};

typedef struct figura* Figura;

Figura newFigura();