#include <cstring>
#include <vector>
#include "figura.h"

struct figura {
    std::vector<Vertice> vertices;
};

Figura newFigura() {
    Figura novaFigura = new struct figura;
    return novaFigura;
}

// Getter para os vértices da figura
std::vector<Vertice> getFiguraVertices(Figura figura) {
    return figura->vertices;
}

// Setter para os vértices da figura
void setFiguraVertices(Figura& figura, const std::vector<Vertice>& vertices) {
    figura->vertices = vertices;
}

Figura fileToFigura(const char* model) {
            string filepath = "../../../outputs/" + string(model);
    		ifstream file(filepath);
    		if (!file) {
    			cerr << "Erro ao abrir o arquivo: " << model << endl;
    			return NULL;
    		}
            cout << "Ficheiro aberto com sucesso!";

    		float x, y, z;
    		vector<Vertice> vertices;
    
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
    
    			vertices.push_back(newVertice(coords[0], coords[1], coords[2]));
    		}
    
    		Figura fig = newFigura();
    		fig->vertices = vertices;
    
    		file.close();

            return fig;
}