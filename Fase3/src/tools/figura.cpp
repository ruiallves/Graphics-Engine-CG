#include <cstring>
#include <vector>
#include "figura.h"

struct figura {
    LinkedList vertices;
};

LinkedList getFiguraVertices(Figura f) {
    if (f) {
        return f->vertices;
    }
    return NULL;
}

size_t getTotalVertices(Figura figura) {
    size_t totalVertices = 0;
    LinkedList vertices = getFiguraVertices(figura);
    LinkedList verticeNode = vertices;

    while (verticeNode != NULL) {
        ++totalVertices;
        verticeNode = (LinkedList)getNext(verticeNode);
    }

    return totalVertices;
}

void setFiguraVertices(Figura figura, LinkedList vertices) {
    figura->vertices = vertices;
}


Figura newFigura() {
    Figura novaFigura = (Figura)malloc(sizeof(struct figura));
    if (novaFigura) {
        novaFigura->vertices = newLinkedListEmpty();
    }
    return novaFigura;
}

vector<float> figuraToVector(Figura f) {
    vector<float> result;
    LinkedList pontos = getFiguraVertices(f);
    LinkedList currentNode = pontos;

    while (currentNode != nullptr) {
        Vertice p = (Vertice)getData(currentNode);
        if (p != nullptr) {
            result.push_back(getVerticeX(p));
            result.push_back(getVerticeY(p));
            result.push_back(getVerticeZ(p));
        }
        currentNode = (LinkedList)getNext(currentNode);
    }

    return result;
}

Figura fileToFigura(const char* model) {
    string filepath = "../../../outputs/" + string(model);
    ifstream file(filepath);
    if (!file) {
        cerr << "Erro ao abrir o arquivo: " << model << endl;
        return NULL;
    }
    cout << "Ficheiro aberto com sucesso!\n";

    float x, y, z;
    LinkedList vertices = newLinkedListEmpty();

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string token;
        float coord;
        vector<float> coords;

        while (getline(iss, token, ',')) {
            istringstream(token) >> coord;
            coords.push_back(coord);
        }

        if (coords.size() != 3) {
            cerr << "Erro ao ler linha do arquivo: " << model << endl;
            continue;
        }

        Vertice vertice = newVertice(coords[0], coords[1], coords[2]);
        addChild(&vertices, vertice);
    }

    //printVerticesLinkedList(vertices);

    Figura fig = newFigura();
    setFiguraVertices(fig, vertices);
    file.close();

    return fig;
}
