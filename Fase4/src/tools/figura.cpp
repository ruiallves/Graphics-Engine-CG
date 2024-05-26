#include <cstring>
#include <vector>
#include "figura.h"

struct figura {
    LinkedList vertices;
    LinkedList normais;
    LinkedList textCoordenadas;
    char* textura;
    LinkedList diffuse;
    LinkedList ambient;
    LinkedList specular;
    LinkedList emissive;
    float shininess;
};

LinkedList getFiguraVertices(Figura f) {
    if (f) {
        return f->vertices;
    }
    return NULL;
}

LinkedList getFiguraNormais(Figura f) {
    if (f) {
		return f->normais;
	}
	return NULL;
}

LinkedList getFiguraTextCoordenadas(Figura f) {
    if (f) {
		return f->textCoordenadas;
	}
	return NULL;
}

char* getFiguraTextura(Figura f) {
    if (f) {
		return f->textura;
	}
	return NULL;
}

LinkedList getFiguraDiffuse(Figura f) {
    if (f) {
		return f->diffuse;
	}
	return NULL;
}

LinkedList getFiguraAmbient(Figura f) {
    if (f) {
		return f->ambient;
	}
	return NULL;
}

LinkedList getFiguraSpecular(Figura f) {
    if (f) {
		return f->specular;
	}
	return NULL;
}

LinkedList getFiguraEmissive(Figura f) {
    if (f) {
		return f->emissive;
	}
	return NULL;
}

float getFiguraShininess(Figura f) {
    if (f) {
		return f->shininess;
	}
	return 0.0f;
}

void setFiguraTextura(Figura f, const char* textura) {
    if (f) {
		f->textura = strdup(textura);
	}
}

void setFiguraDiffuse(Figura f, LinkedList diffuse) {
    if (f) {
		f->diffuse = diffuse;
	}
}

void setFiguraAmbient(Figura f, LinkedList ambient) {
    if (f) {
		f->ambient = ambient;
	}
}

void setFiguraSpecular(Figura f, LinkedList specular) {
    if (f) {
		f->specular = specular;
	}
}

void setFiguraEmissive(Figura f, LinkedList emissive) {
    if (f) {
		f->emissive = emissive;
	}
}

void setFiguraShininess(Figura f, float shininess) {
    if (f) {
		f->shininess = shininess;
	}
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

void setFiguraNormals(Figura figura, LinkedList normais) {
	figura->normais = normais;
}

void setFiguraTexCoords(Figura figura, LinkedList textCoordenadas) {
	figura->textCoordenadas = textCoordenadas;
}


Figura newFigura() {
    Figura novaFigura = (Figura)malloc(sizeof(struct figura));
    if (novaFigura) {
        novaFigura->vertices = newLinkedListEmpty();
        novaFigura->normais = newLinkedListEmpty();
        novaFigura->textCoordenadas = newLinkedListEmpty();
        novaFigura->textura = NULL;
        novaFigura->diffuse = newLinkedListEmpty();
        novaFigura->ambient = newLinkedListEmpty();
        novaFigura->specular = newLinkedListEmpty();
        novaFigura->emissive = newLinkedListEmpty();
        novaFigura->shininess = 0.0f;

        //valores defaut
        addChild(&novaFigura->diffuse, static_cast<void*>(new float(200.0f)));
        addChild(&novaFigura->diffuse, static_cast<void*>(new float(200.0f)));
        addChild(&novaFigura->diffuse, static_cast<void*>(new float(200.0f)));
        addChild(&novaFigura->diffuse, static_cast<void*>(new float(1.0f)));

        addChild(&novaFigura->ambient, static_cast<void*>(new float(50.0f)));
        addChild(&novaFigura->ambient, static_cast<void*>(new float(50.0f)));
        addChild(&novaFigura->ambient, static_cast<void*>(new float(50.0f)));
        addChild(&novaFigura->ambient, static_cast<void*>(new float(1.0f)));

        addChild(&novaFigura->specular, static_cast<void*>(new float(0.0f)));
        addChild(&novaFigura->specular, static_cast<void*>(new float(0.0f)));
        addChild(&novaFigura->specular, static_cast<void*>(new float(0.0f)));
        addChild(&novaFigura->specular, static_cast<void*>(new float(1.0f)));

        addChild(&novaFigura->emissive, static_cast<void*>(new float(0.0f)));
        addChild(&novaFigura->emissive, static_cast<void*>(new float(0.0f)));
        addChild(&novaFigura->emissive, static_cast<void*>(new float(0.0f)));
        addChild(&novaFigura->emissive, static_cast<void*>(new float(1.0f)));
        novaFigura->shininess = 0.0f;

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

vector<float> figuraNormalsToVector(Figura f) {
    	vector<float> result;
	LinkedList pontos = getFiguraNormais(f);
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

vector<float> figuraTexturesToVector(Figura f, const char* textFile) {
	vector<float> result;
	LinkedList pontos = getFiguraTextCoordenadas(f);
	LinkedList currentNode = pontos;

    while (currentNode != nullptr) {
		Vertice p = (Vertice)getData(currentNode);
        if (p != nullptr) {
			result.push_back(getVerticeX(p));
			result.push_back(getVerticeY(p));
		}
		currentNode = (LinkedList)getNext(currentNode);
	}

	return result;
}

Figura fileToFigura(const char* model) {
    std::string filepath = "../../../outputs/" + std::string(model);
    std::ifstream file(filepath);
    if (!file) {
        std::cerr << "Erro ao abrir o arquivo: " << model << std::endl;
        return nullptr;
    }
    std::cout << "Ficheiro aberto com sucesso!\n";

    LinkedList vertices = newLinkedListEmpty();
    LinkedList normals = newLinkedListEmpty();
    LinkedList texCoords = newLinkedListEmpty();

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        float coord;

        std::vector<float> coords;

        // Lê os valores da linha
        while (std::getline(iss, token, ',')) {
            std::istringstream(token) >> coord;
            coords.push_back(coord);
        }

        // Verifica se há o número correto de valores na linha
        if (coords.size() != 8) {
            std::cerr << "Erro ao ler linha do arquivo: " << model << std::endl;
            continue;
        }

        // Adiciona os valores aos respectivos conjuntos
        Vertice vertice = newVertice(coords[0], coords[1], coords[2]);
        addChild(&vertices, vertice);

        Vertice normal = newVertice(coords[3], coords[4], coords[5]);
        addChild(&normals, normal);

        Vertice texCoord = newVertice(coords[6], coords[7], 0.0f);
        addChild(&texCoords, texCoord);
    }

    Figura fig = newFigura();
    setFiguraVertices(fig, vertices);
    setFiguraNormals(fig, normals);
    setFiguraTexCoords(fig, texCoords);

    file.close();

    return fig;
}

vector<float> getDif(Figura f) {
    vector<float> result;
	LinkedList dif = getFiguraDiffuse(f);
	LinkedList currentNode = dif;

    while (currentNode != nullptr) {
		float* p = (float*)getData(currentNode);
        if (p != nullptr) {
			result.push_back(*p/255.0f);
		}
		currentNode = (LinkedList)getNext(currentNode);
	}

	return result;
}

vector<float> getAmb(Figura f) {
	vector<float> result;
	LinkedList amb = getFiguraAmbient(f);
	LinkedList currentNode = amb;

    while (currentNode != nullptr) {
		float* p = (float*)getData(currentNode);
        if (p != nullptr) {
            result.push_back(*p / 255.0f);
		}
		currentNode = (LinkedList)getNext(currentNode);
	}

	return result;
}

vector<float> getSpec(Figura f) {
	vector<float> result;
	LinkedList spec = getFiguraSpecular(f);
	LinkedList currentNode = spec;

    while (currentNode != nullptr) {
		float* p = (float*)getData(currentNode);
        if (p != nullptr) {
            result.push_back(*p / 255.0f);
		}
		currentNode = (LinkedList)getNext(currentNode);
	}

	return result;
}

vector<float> getEmi(Figura f) {
	vector<float> result;
	LinkedList emi = getFiguraEmissive(f);
	LinkedList currentNode = emi;

    while (currentNode != nullptr) {
		float* p = (float*)getData(currentNode);
        if (p != nullptr) {
            result.push_back(*p / 255.0f);
		}
		currentNode = (LinkedList)getNext(currentNode);
	}

	return result;
}