#include "createvertice.h"

#define M_PI 3.14159265358979323846

Figura createPlane(int length, int divisions) {
    // Cria um novo objeto do tipo Figura para representar o plano
    Figura plane = newFigura();

    // Verifica se a alocação de memória foi bem-sucedida
    if (plane) {
        // Calcula a metade do comprimento do plano
        float dimension2 = static_cast<float>(length) / 2;

        // Calcula o tamanho de cada divisão do plano
        float div_side = static_cast<float>(length) / divisions;

        // Inicializa as coordenadas do canto inferior esquerdo do plano
        float x1 = -dimension2, z1 = -dimension2;

        // Loop para percorrer as linhas do plano
        for (int linha = 0; linha < divisions; linha++) {
            // Loop para percorrer as colunas do plano
            for (int coluna = 0; coluna < divisions; coluna++) {
                // Adiciona os vértices do primeiro triângulo do quadrado
                plane->vertices.push_back(newVertice(x1 + coluna * div_side, 0.0f, z1));
                plane->vertices.push_back(newVertice(x1 + coluna * div_side + div_side, 0.0f, z1));
                plane->vertices.push_back(newVertice(x1 + coluna * div_side + div_side, 0.0f, z1 + div_side));

                // Adiciona os vértices do segundo triângulo do quadrado
                plane->vertices.push_back(newVertice(x1 + coluna * div_side, 0.0f, z1));
                plane->vertices.push_back(newVertice(x1 + coluna * div_side + div_side, 0.0f, z1 + div_side));
                plane->vertices.push_back(newVertice(x1 + coluna * div_side, 0.0f, z1 + div_side));
            }
            // Avança para a próxima linha do plano
            z1 += div_side;
        }
    }
    // Retorna o plano criado
    return plane;
}


Figura createCone(float radius, float height, int slices, int stacks) {
    // Cria um novo objeto do tipo Figura para representar o cone
    Figura cone = newFigura();

    // Calcula o ângulo inicial
    float theta = 0;
    // Variável para armazenar o próximo ângulo
    float nextTheta = 0;
    // Calcula o ângulo de variação para cada fatia
    float delta = (2 * M_PI) / slices;
    // Calcula o raio de cada fatia do cone
    float raio = radius / stacks;
    // Calcula a altura de cada fatia do cone
    float alturas = height / stacks;
    // Variáveis para iteração
    int i, j;

    // Desenha a circunferência da base do cone
    for (i = 0; i < slices; i++) {
        // Calcula o próximo ângulo
        nextTheta = theta + delta;

        // Adiciona os vértices do triângulo formado pela base do cone
        cone->vertices.push_back(newVertice(0, 0, 0));
        cone->vertices.push_back(newVertice(radius * sin(nextTheta), 0, radius * cos(nextTheta)));
        cone->vertices.push_back(newVertice(radius * sin(theta), 0, radius * cos(theta)));

        // Atualiza o valor de theta
        theta = nextTheta;
    }

    // Desenha as laterais do cone
    float r1 = radius;
    float r2 = radius - raio;
    float alt1 = 0;
    float alt2 = alturas;
    theta = 0;
    nextTheta = 0;

    for (i = 0; i < slices; i++) {
        nextTheta += delta;

        // Desenha as fatias do cone
        for (j = 0; j < stacks; j++) {
            // Adiciona os vértices dos triângulos formados pelas laterais do cone
            cone->vertices.push_back(newVertice(r1 * sin(nextTheta), alt1, r1 * cos(nextTheta)));
            cone->vertices.push_back(newVertice(r2 * sin(nextTheta), alt2, r2 * cos(nextTheta)));
            cone->vertices.push_back(newVertice(r1 * sin(theta), alt1, r1 * cos(theta)));

            cone->vertices.push_back(newVertice(r2 * sin(nextTheta), alt2, r2 * cos(nextTheta)));
            cone->vertices.push_back(newVertice(r2 * sin(theta), alt2, r2 * cos(theta)));
            cone->vertices.push_back(newVertice(r1 * sin(theta), alt1, r1 * cos(theta)));

            // Atualiza os valores de raio e altura
            r1 -= raio;
            r2 -= raio;
            alt1 += alturas;
            alt2 += alturas;
        }
        // Reinicializa os valores para a próxima fatia
        r1 = radius;
        r2 = radius - raio;
        alt1 = 0;
        alt2 = alturas;
        theta = nextTheta;
    }
    // Retorna o cone criado
    return cone;
}


Figura createBox(float size, int divisions) {
    Figura box = newFigura();

    // Calcula o tamanho de cada divisão
    float step = size / divisions;

    // Loop para desenhar as faces do cubo
    for (int i = 0; i < divisions; i++) {
        for (int j = 0; j < divisions; j++) {
            // Calcula as coordenadas dos vértices de um quadrado que forma uma das faces do cubo
            float x1 = -size / 2 + i * step;  // Canto inferior esquerdo no eixo x
            float z1 = -size / 2 + j * step;  // Canto inferior esquerdo no eixo z
            float y1 = size / 2;               // Altura da face superior

            float x2 = -size / 2 + (i + 1) * step;  // Canto inferior direito no eixo x
            float z2 = -size / 2 + j * step;        // Canto inferior direito no eixo z
            float y2 = size / 2;                    // Altura da face superior

            float x3 = -size / 2 + i * step;     // Canto superior esquerdo no eixo x
            float z3 = -size / 2 + (j + 1) * step;  // Canto superior esquerdo no eixo z
            float y3 = size / 2;                    // Altura da face superior

            float x4 = -size / 2 + (i + 1) * step;  // Canto superior direito no eixo x
            float z4 = -size / 2 + (j + 1) * step;  // Canto superior direito no eixo z
            float y4 = size / 2;                    // Altura da face superior

            // Adiciona os vértices do quadrado à lista de vértices da caixa
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
