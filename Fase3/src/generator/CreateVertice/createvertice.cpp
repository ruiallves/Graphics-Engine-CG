#include "createvertice.h"
#include <cmath>

#define M_PI 3.14159265358979323846

Figura createPlane(int length, int divisions) {
    Figura plane = newFigura();
    LinkedList vertices = newLinkedListEmpty();
    if (plane) {
        float dimension2 = static_cast<float>(length) / 2;
        float div_side = static_cast<float>(length) / divisions;
        float x1 = -dimension2, z1 = -dimension2;
        for (int linha = 0; linha < divisions; linha++) {
            for (int coluna = 0; coluna < divisions; coluna++) {
                // Primeiro triângulo do quadrado
                addChild(&vertices, newVertice(x1 + coluna * div_side, 0.0f, z1));
                addChild(&vertices, newVertice(x1 + coluna * div_side + div_side, 0.0f, z1));
                addChild(&vertices, newVertice(x1 + coluna * div_side + div_side, 0.0f, z1 + div_side));
                // Segundo triângulo do quadrado
                addChild(&vertices, newVertice(x1 + coluna * div_side, 0.0f, z1));
                addChild(&vertices, newVertice(x1 + coluna * div_side + div_side, 0.0f, z1 + div_side));
                addChild(&vertices, newVertice(x1 + coluna * div_side, 0.0f, z1 + div_side));
            }
            z1 += div_side;
        }
    }
    setFiguraVertices(plane, vertices);
    return plane;
}


Figura createCone(float radius, float height, int slices, int stacks) {
    Figura cone = newFigura();
    LinkedList vertices = newLinkedListEmpty();
    float theta = 0;
    float nextTheta = 0;
    float delta = (2 * M_PI) / slices;
    float raio = radius / stacks;
    float alturas = height / stacks;
    int i, j;

    // Fazer a circunferência da base
    for (i = 0; i < slices; i++) {
        nextTheta = theta + delta;

        addChild(&vertices, newVertice(0, 0, 0));
        addChild(&vertices, newVertice(radius * sin(nextTheta), 0, radius * cos(nextTheta)));
        addChild(&vertices, newVertice(radius * sin(theta), 0, radius * cos(theta)));

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
            addChild(&vertices, newVertice(r1 * sin(nextTheta), alt1, r1 * cos(nextTheta)));
            addChild(&vertices, newVertice(r2 * sin(nextTheta), alt2, r2 * cos(nextTheta)));
            addChild(&vertices, newVertice(r1 * sin(theta), alt1, r1 * cos(theta)));

            addChild(&vertices, newVertice(r2 * sin(nextTheta), alt2, r2 * cos(nextTheta)));
            addChild(&vertices, newVertice(r2 * sin(theta), alt2, r2 * cos(theta)));
            addChild(&vertices, newVertice(r1 * sin(theta), alt1, r1 * cos(theta)));

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
    setFiguraVertices(cone, vertices);

    return cone;
}

Figura createSphere(float radius, float slices, float stacks) {
    Figura esfera = newFigura();
    LinkedList vertices = newLinkedListEmpty();

    float delta1 = M_PI / stacks;
    float delta2 = 2 * M_PI / slices;

    for (float i = -M_PI / 2; i <= M_PI / 2; i += delta1) {
        float phi = i;
        float phiNext = phi + delta1;

        for (float j = 0; j <= 2 * M_PI; j += delta2) {
            float theta = j;
            float thetaNext = theta + delta2;

            addChild(&vertices, newVertice(cos(phi) * sin(theta) * radius, sin(phi) * radius, cos(phi) * cos(theta) * radius));
            addChild(&vertices, newVertice(cos(phiNext) * sin(theta) * radius, sin(phiNext) * radius, cos(phiNext) * cos(theta) * radius));
            addChild(&vertices, newVertice(cos(phiNext) * sin(thetaNext) * radius, sin(phiNext) * radius, cos(phiNext) * cos(thetaNext) * radius));

            addChild(&vertices, newVertice(cos(phi) * sin(theta) * radius, sin(phi) * radius, cos(phi) * cos(theta) * radius));
            addChild(&vertices, newVertice(cos(phiNext) * sin(thetaNext) * radius, sin(phiNext) * radius, cos(phiNext) * cos(thetaNext) * radius));
            addChild(&vertices, newVertice(cos(phi) * sin(thetaNext) * radius, sin(phi) * radius, cos(phi) * cos(thetaNext) * radius));
        }
    }

    setFiguraVertices(esfera, vertices);
    return esfera;
}

Figura createBox(float size, int divisions) {
    Figura box = newFigura();
    LinkedList vertices = newLinkedListEmpty();
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

            addChild(&vertices, newVertice(x3, y3, z3));
            addChild(&vertices, newVertice(x2, y2, z2));
            addChild(&vertices, newVertice(x1, y1, z1));

            addChild(&vertices, newVertice(x3, y3, z3));
            addChild(&vertices, newVertice(x4, y4, z4));
            addChild(&vertices, newVertice(x2, y2, z2));

            addChild(&vertices, newVertice(x3, -y3, z3));
            addChild(&vertices, newVertice(x1, -y1, z1));
            addChild(&vertices, newVertice(x2, -y2, z2));

            addChild(&vertices, newVertice(x3, -y3, z3));
            addChild(&vertices, newVertice(x2, -y2, z2));
            addChild(&vertices, newVertice(x4, -y4, z4));
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

            addChild(&vertices, newVertice(x3, y3, -z3));
            addChild(&vertices, newVertice(x2, y2, -z2));
            addChild(&vertices, newVertice(x1, y1, -z1));

            addChild(&vertices, newVertice(x3, y3, -z3));
            addChild(&vertices, newVertice(x4, y4, -z4));
            addChild(&vertices, newVertice(x2, y2, -z2));

            addChild(&vertices, newVertice(x3, y3, z3));
            addChild(&vertices, newVertice(x1, y1, z1));
            addChild(&vertices, newVertice(x2, y2, z2));

            addChild(&vertices, newVertice(x3, y3, z3));
            addChild(&vertices, newVertice(x2, y2, z2));
            addChild(&vertices, newVertice(x4, y4, z4));
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

            addChild(&vertices, newVertice(-x3, y3, z3));
            addChild(&vertices, newVertice(-x1, y1, z1));
            addChild(&vertices, newVertice(-x2, y2, z2));

            addChild(&vertices, newVertice(-x3, y3, z3));
            addChild(&vertices, newVertice(-x2, y2, z2));
            addChild(&vertices, newVertice(-x4, y4, z4));

            addChild(&vertices, newVertice(x3, y3, -z3));
            addChild(&vertices, newVertice(x1, y1, -z1));
            addChild(&vertices, newVertice(x2, y2, -z2));

            addChild(&vertices, newVertice(x3, y3, -z3));
            addChild(&vertices, newVertice(x2, y2, -z2));
            addChild(&vertices, newVertice(x4, y4, -z4));
        }
    }

    setFiguraVertices(box, vertices);
    return box;
}


Figura createRing(float ri, float re, int slices) {
    Figura ring = newFigura();
    LinkedList vertices = newLinkedListEmpty();
    float a = 0, delta = (2 * M_PI) / slices;
    if (ring) {
        for (int i = 0; i < slices; i++, a += delta) {
            addChild(&vertices, newVerticeSph(a, 0.0f, ri));
            addChild(&vertices, newVerticeSph(a, 0.0f, re));
            addChild(&vertices, newVerticeSph(a + delta, 0.0f, ri));

            addChild(&vertices, newVerticeSph(a + delta, 0.0f, ri));
            addChild(&vertices, newVerticeSph(a, 0.0f, re));
            addChild(&vertices, newVerticeSph(a + delta, 0.0f, re));

            addChild(&vertices, newVerticeSph(a + delta, 0.0f, ri));
            addChild(&vertices, newVerticeSph(a, 0.0f, re));
            addChild(&vertices, newVerticeSph(a, 0.0f, ri));

            addChild(&vertices, newVerticeSph(a, 0.0f, re));
            addChild(&vertices, newVerticeSph(a + delta, 0.0f, ri));
            addChild(&vertices, newVerticeSph(a + delta, 0.0f, re));
        }
    }
    setFiguraVertices(ring, vertices);

    return ring;
}

// Função para calcular a fórmula de Bezier
float* bezierFormula(float t, float* p0, float* p1, float* p2, float* p3) {
    float* result = new float[3];

    float oneMinusT = 1.0f - t;
    float oneMinusTSquared = oneMinusT * oneMinusT;
    float oneMinusTCubed = oneMinusTSquared * oneMinusT;
    float tSquared = t * t;
    float tCubed = tSquared * t;

    // Cálculo de x, y e z usando a fórmula de Bezier
    for (int i = 0; i < 3; ++i) {
        result[i] = oneMinusTCubed * p0[i] + 3 * oneMinusTSquared * t * p1[i] + 3 * oneMinusT * tSquared * p2[i] + tCubed * p3[i];
    }

    return result;
}

float* bezier(float n, float m, float** points, int* index) {
    float* result = new float[3];
    float* point = new float[3];
    float pointsAcumulator[4][3];
    float bezierAcumulator[4][3];

    int bezierIndex = 0;

    for (int i = 0; i < 16; ++i) {
        int pointIndex = index[i];
        int pointInQuadruple = i % 4;

        for (int j = 0; j < 3; ++j) {
            pointsAcumulator[pointInQuadruple][j] = points[pointIndex][j];
        }

        if (pointInQuadruple == 3) {
            point = bezierFormula(n, pointsAcumulator[0], pointsAcumulator[1], pointsAcumulator[2], pointsAcumulator[3]);

            for (int j = 0; j < 3; ++j) {
                bezierAcumulator[bezierIndex][j] = point[j];
            }

            ++bezierIndex;
        }
    }

    result = bezierFormula(m, bezierAcumulator[0], bezierAcumulator[1], bezierAcumulator[2], bezierAcumulator[3]);

    return result;
}

Figura createBezier(std::string ReadFile, int tessellation) {
    std::ifstream read("../../../patches/" + ReadFile + ".patch");
    Figura fig = newFigura();
    LinkedList vertices = newLinkedListEmpty();

    std::string line, value;
    int i1, i2, p1, position;
    std::string delimiter = ",";

    float increment = 1.0f / tessellation;

    if (read.is_open()) {
        getline(read, line);
        int nPatches = std::stoi(line);
        int** index = new int* [nPatches];
        float** points = nullptr;
        int numControlPoints = 0;

        for (i1 = 0; i1 < nPatches; ++i1) {
            getline(read, line);
            index[i1] = new int[16];

            std::istringstream ss(line);
            std::string token;

            for (p1 = 0; p1 < 16; ++p1) {
                getline(ss, token, ',');
                index[i1][p1] = std::stoi(token);
            }
        }

        getline(read, line);
        numControlPoints = std::stoi(line);
        points = new float* [numControlPoints];

        for (i2 = 0; i2 < numControlPoints; ++i2) {
            getline(read, line);
            points[i2] = new float[3];

            std::istringstream ss(line);
            std::string token;

            for (p1 = 0; p1 < 3; ++p1) {
                getline(ss, token, ',');
                points[i2][p1] = std::stof(token);
            }
        }

        for (i1 = 0; i1 < nPatches; ++i1) {
            for (int xx = 0; xx < tessellation; ++xx) {
                for (int yy = 0; yy < tessellation; ++yy) {
                    float u1 = increment * xx;
                    float u2 = increment * (xx + 1);
                    float v1 = increment * yy;
                    float v2 = increment * (yy + 1);

                    float* a = bezier(u1, v1, points, index[i1]);
                    float* b = bezier(u2, v1, points, index[i1]);
                    float* c = bezier(u1, v2, points, index[i1]);
                    float* d = bezier(u2, v2, points, index[i1]);

                    // Triângulo 1  
                    addChild(&vertices, newVertice(a[0], a[1], a[2]));
                    addChild(&vertices, newVertice(b[0], b[1], b[2]));
                    addChild(&vertices, newVertice(c[0], c[1], c[2]));

                    // Triângulo 2
                    addChild(&vertices, newVertice(b[0], b[1], b[2]));
                    addChild(&vertices, newVertice(d[0], d[1], d[2]));
                    addChild(&vertices, newVertice(c[0], c[1], c[2]));

                    delete[] a;
                    delete[] b;
                    delete[] c;
                    delete[] d;
                }
            }
        }

        setFiguraVertices(fig, vertices);

        // Liberar memória
        for (i1 = 0; i1 < nPatches; ++i1) {
            delete[] index[i1];
        }
        delete[] index;

        for (i2 = 0; i2 < numControlPoints; ++i2) {
            delete[] points[i2];
        }
        delete[] points;

        read.close();

        return fig;
    }
    else {
        std::cerr << "Erro ao abrir o arquivo de entrada: " << ReadFile << std::endl;
        return nullptr;
    }

    return nullptr;
}