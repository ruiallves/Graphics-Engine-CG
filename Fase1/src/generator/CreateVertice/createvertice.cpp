#include "createvertice.h"

Figura createPlane(float x, float z) {

    Figura figura = newFigura();

    float x1 = x / 2;
    float z1 = z / 2;

    //Triângulo 1
    figura->vertices.push_back(newVertice(x1, 0, z1));
    figura->vertices.push_back(newVertice(x1, 0, -z1));
    figura->vertices.push_back(newVertice(-x1, 0, -z1));

    // Triângulo 2
    figura->vertices.push_back(newVertice(-x1, 0, -z1));
    figura->vertices.push_back(newVertice(-x1, 0, z1));
    figura->vertices.push_back(newVertice(x1, 0, z1));

    return figura;
}
