#include "modelo.hpp"

struct modelo {

    Colecao pontos; // lista de pontos

};

Modelo newEmptyModelo()
{

    Modelo mod = (Modelo)malloc(sizeof(struct modelo));

    if (mod != NULL)
	{

		mod->pontos = newEmptyColecao();
		return mod;

	}

	else
	{

		return NULL;

	}

}

Modelo newModelo(Colecao pontos) {
    Modelo m = newEmptyModelo();

    if (m != NULL) {

        for (unsigned long i = 0; i < getColecaoLength(pontos); i++) {
            addPonto(m, (Ponto)getColecaoEle(pontos, i));
        }

    }

    return m;
}

void addPonto(Modelo m, Ponto p) {
    if (m) {
        addValueColecao(m->pontos, p);
    }
}

void ModeloToFile(Modelo mod, const char* path)
{

    if (!mod) {

        printf("Modelo vazio!\n");
        return;
    
    }

	FILE* f = fopen(path, "w");

    if (!f) {
    
        printf("Erro ao abrir o ficheiro!\n");
		return;

    }

    fprintf(f, "%lu\n", getColecaoLength(mod->pontos));
    for (unsigned long i = 0; i < getColecaoLength(mod->pontos); i++) {
        Ponto p = (Ponto)getColecaoEle(mod->pontos, i);
        fprintf(f, "%g,%g,%g\n", getX(p), getY(p), getZ(p));
    }
    fclose(f);

}

Modelo fileToModelo(const char* path){

    Modelo mod = newEmptyModelo();
    FILE *f = fopen(path, "r");

    if (mod && f) {

        char buffer[8192];
        fgets(buffer, 8191, f);
        int vertices = atoi(buffer);
        float x, y, z;

        for (int i = 0; i < vertices; i++) {
            fgets(buffer, 8191, f);
            sscanf(buffer, "%f,%f,%f", &x, &y, &z);
            addPonto(mod, newPonto(x, y, z));
        }

        fclose(f);

    }

    return mod;

}

Colecao getPontos(Modelo mod){
    return mod ? mod->pontos : NULL;
}

void delModelo(void* mod){

    if (mod) {

        for (unsigned long i = 0; i < getColecaoLength(((Modelo)mod)->pontos); i++) {
			DelPonto((Ponto)getColecaoEle(((Modelo)mod)->pontos, i));
        }
        delColecao(((Modelo)mod)->pontos);
		free(mod);
    }

}