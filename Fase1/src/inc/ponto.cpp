#include "ponto.hpp"

struct ponto {
    float x, y, z;
};

float getX(Ponto p){ return p ? p->x : 0.0f; }

float getY(Ponto p){ return p ? p->y : 0.0f; }

float getZ(Ponto p){ return p ? p->z : 0.0f; }

Ponto newEmptyPonto() { return newPonto(0.0f, 0.0f, 0.0f); }

Ponto newPonto(float x, float y, float z)
{
    Ponto p = (Ponto)malloc(sizeof(struct ponto));

    if (p) {
        p->x = x;
        p->y = y;
        p->z = z;
        return p;
    }

	return NULL;

}

Ponto newPontoSPHERE(float a, float b, float radius)
{

    float x = radius * cos(b) * sin(a);
    float y = radius * sin(b);
    float z = radius * cos(b) * cos(a);

    return newPonto(x, y, z);

}

float distance(Ponto p1, Ponto p2)
{
    float dx = getX(p2) - getX(p1);
    float dy = getY(p2) - getY(p1);
    float dz = getZ(p2) - getZ(p1);
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

float distanceToOrigin(Ponto p)
{
    return 0.0f;
}

void DelPonto(Ponto p){ free(p); }
