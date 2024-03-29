#include "createvertice.h"

#define M_PI 3.14159265358979323846

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

Figura createCone(float radius, float height, int slices, int stacks) {
    Figura cone = newFigura();

    float theta = 0;
    float nextTheta = 0;
    float delta = (2 * M_PI) / slices;
    float raio = radius / stacks;
    float alturas = height / stacks;
    int i, j;

    // Fazer a circunferência da base
    for (i = 0; i < slices; i++) {
        nextTheta = theta + delta;

        cone->vertices.push_back(newVertice(0, 0, 0));
        cone->vertices.push_back(newVertice(radius * sin(nextTheta), 0, radius * cos(nextTheta)));
        cone->vertices.push_back(newVertice(radius * sin(theta), 0, radius * cos(theta)));

        theta = nextTheta;
    }

    // Fazer as laterais
    float r1 = radius;
    float r2 = radius - raio;
    float alt1 = 0;
    float alt2 = alturas;
    theta = 0;
    nextTheta = 0;

    for (i = 0; i < slices; i++) {
        nextTheta += delta;

        for (j = 0; j < stacks; j++) {
            cone->vertices.push_back(newVertice(r1 * sin(nextTheta), alt1, r1 * cos(nextTheta)));
            cone->vertices.push_back(newVertice(r2 * sin(nextTheta), alt2, r2 * cos(nextTheta)));
            cone->vertices.push_back(newVertice(r1 * sin(theta), alt1, r1 * cos(theta)));

            cone->vertices.push_back(newVertice(r2 * sin(nextTheta), alt2, r2 * cos(nextTheta)));
            cone->vertices.push_back(newVertice(r2 * sin(theta), alt2, r2 * cos(theta)));
            cone->vertices.push_back(newVertice(r1 * sin(theta), alt1, r1 * cos(theta)));

            r1 -= raio;
            r2 -= raio;
            alt1 += alturas;
            alt2 += alturas;
        }
        r1 = radius;
        r2 = radius - raio;
        alt1 = 0;
        alt2 = alturas;
        theta = nextTheta;
    }
    return cone;
}

Figura createBox(float size, int divisions) {
    Figura box = newFigura();
    float step = size / divisions;

    for (int i = 0; i < divisions; i++) {
        for (int j = 0; j < divisions; j++) {
            float x1 = -size / 2 + i * step;
            float z1 = -size / 2 + j * step;
            float y1 = size / 2;

            float x2 = -size / 2 + (i + 1) * step;
            float z2 = -size / 2 + j * step;
            float y2 = size / 2;

            float x3 = -size / 2 + i * step;
            float z3 = -size / 2 + (j + 1) * step;
            float y3 = size / 2;

            float x4 = -size / 2 + (i + 1) * step;
            float z4 = -size / 2 + (j + 1) * step;
            float y4 = size / 2;

            box->vertices.push_back(newVertice(x3, y3, z3));
            box->vertices.push_back(newVertice(x2, y2, z2));
            box->vertices.push_back(newVertice(x1, y1, z1));

            box->vertices.push_back(newVertice(x3, y3, z3));
            box->vertices.push_back(newVertice(x4, y4, z4));
            box->vertices.push_back(newVertice(x2, y2, z2));

            box->vertices.push_back(newVertice(x3, -y3, z3));
            box->vertices.push_back(newVertice(x1, -y1, z1));
            box->vertices.push_back(newVertice(x2, -y2, z2));

            box->vertices.push_back(newVertice(x3, -y3, z3));
            box->vertices.push_back(newVertice(x2, -y2, z2));
            box->vertices.push_back(newVertice(x4, -y4, z4));
        }
    }

    for (int i = 0; i < divisions; i++) {
        for (int j = 0; j < divisions; j++) {
            float x1 = -size / 2 + i * step;
            float y1 = -size / 2 + j * step;
            float z1 = size / 2;

            float x2 = -size / 2 + (i + 1) * step;
            float y2 = -size / 2 + j * step;
            float z2 = size / 2;

            float x3 = -size / 2 + i * step;
            float y3 = -size / 2 + (j + 1) * step;
            float z3 = size / 2;

            float x4 = -size / 2 + (i + 1) * step;
            float y4 = -size / 2 + (j + 1) * step;
            float z4 = size / 2;

            box->vertices.push_back(newVertice(x3, y3, -z3));
            box->vertices.push_back(newVertice(x2, y2, -z2));
            box->vertices.push_back(newVertice(x1, y1, -z1));

            box->vertices.push_back(newVertice(x3, y3, -z3));
            box->vertices.push_back(newVertice(x4, y4, -z4));
            box->vertices.push_back(newVertice(x2, y2, -z2));

            box->vertices.push_back(newVertice(x3, y3, z3));
            box->vertices.push_back(newVertice(x1, y1, z1));
            box->vertices.push_back(newVertice(x2, y2, z2));

            box->vertices.push_back(newVertice(x3, y3, z3));
            box->vertices.push_back(newVertice(x2, y2, z2));
            box->vertices.push_back(newVertice(x4, y4, z4));
        }
    }

    for (int i = 0; i < divisions; i++) {
        for (int j = 0; j < divisions; j++) {
            float z1 = -size / 2 + i * step;
            float y1 = -size / 2 + j * step;
            float x1 = size / 2;

            float z2 = -size / 2 + (i + 1) * step;
            float y2 = -size / 2 + j * step;
            float x2 = size / 2;

            float z3 = -size / 2 + i * step;
            float y3 = -size / 2 + (j + 1) * step;
            float x3 = size / 2;

            float z4 = -size / 2 + (i + 1) * step;
            float y4 = -size / 2 + (j + 1) * step;
            float x4 = size / 2;

            box->vertices.push_back(newVertice(-x3, y3, z3));
            box->vertices.push_back(newVertice(-x1, y1, z1));
            box->vertices.push_back(newVertice(-x2, y2, z2));

            box->vertices.push_back(newVertice(-x3, y3, z3));
            box->vertices.push_back(newVertice(-x2, y2, z2));
            box->vertices.push_back(newVertice(-x4, y4, z4));

            box->vertices.push_back(newVertice(x3, y3, -z3));
            box->vertices.push_back(newVertice(x1, y1, -z1));
            box->vertices.push_back(newVertice(x2, y2, -z2));

            box->vertices.push_back(newVertice(x3, y3, -z3));
            box->vertices.push_back(newVertice(x2, y2, -z2));
            box->vertices.push_back(newVertice(x4, y4, -z4));
        }
    }

    return box;
}

Figura createSphere(float radius, float slices, float stacks) {
    Figura esfera = newFigura();

    float delta1 = M_PI / stacks;
    float delta2 = 2 * M_PI / slices;

    for (float i = -M_PI / 2; i <= M_PI / 2; i += delta1) {
        float phi = i;
        float phiNext = phi + delta1;

        for (float j = 0; j <= 2 * M_PI; j += delta2) {
            float theta = j;
            float thetaNext = theta + delta2;

            float x1 = cos(phi) * sin(theta) * radius;
            float y1 = sin(phi) * radius;
            float z1 = cos(phi) * cos(theta) * radius;

            float x2 = cos(phiNext) * sin(theta) * radius;
            float y2 = sin(phiNext) * radius;
            float z2 = cos(phiNext) * cos(theta) * radius;

            float x3 = cos(phiNext) * sin(thetaNext) * radius;
            float y3 = sin(phiNext) * radius;
            float z3 = cos(phiNext) * cos(thetaNext) * radius;

            float x4 = cos(phi) * sin(thetaNext) * radius;
            float y4 = sin(phi) * radius;
            float z4 = cos(phi) * cos(thetaNext) * radius;

            esfera->vertices.push_back(newVertice(x1, y1, z1));
            esfera->vertices.push_back(newVertice(x2, y2, z2));
            esfera->vertices.push_back(newVertice(x3, y3, z3));

            esfera->vertices.push_back(newVertice(x1, y1, z1));
            esfera->vertices.push_back(newVertice(x3, y3, z3));
            esfera->vertices.push_back(newVertice(x4, y4, z4));
        }
    }

    return esfera;
}
