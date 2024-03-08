#include "createvertice.h"

Figura createPlane(int length, int divisions) {
    Figura plane = newFigura();
    if (plane) {
        float dimension2 = static_cast<float>(length) / 2;
        float div_side = static_cast<float>(length) / divisions;
        float x1 = -dimension2, z1 = -dimension2;

        for (int linha = 0; linha < divisions; linha++) {
            for (int coluna = 0; coluna < divisions; coluna++) {
                // Primeiro triângulo do quadrado
                plane->vertices.push_back(newVertice(x1 + coluna * div_side, 0.0f, z1));
                plane->vertices.push_back(newVertice(x1 + coluna * div_side + div_side, 0.0f, z1));
                plane->vertices.push_back(newVertice(x1 + coluna * div_side + div_side, 0.0f, z1 + div_side));
                // Segundo triângulo do quadrado
                plane->vertices.push_back(newVertice(x1 + coluna * div_side, 0.0f, z1));
                plane->vertices.push_back(newVertice(x1 + coluna * div_side + div_side, 0.0f, z1 + div_side));
                plane->vertices.push_back(newVertice(x1 + coluna * div_side, 0.0f, z1 + div_side));
            }
            z1 += div_side;
        }
    }
    return plane;
}


