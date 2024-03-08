#include "colecaoPontos.hpp"

struct colecao{

    void** col; // colecao de apontadores
	unsigned long length; // tamanho da lista
    unsigned long capacity; // capacidade da lista

};

Colecao newEmptyColecao() {

    Colecao novacolecao = (Colecao)malloc(sizeof(struct colecao));
    
    if (novacolecao) { // não ocorreram erros a criar a novacolecao

        novacolecao->col = (void**)calloc(DEFAULTSIZE, sizeof(void*)); //calloc para ficar tudo a zero's (controlo de mem.)
    
        if (novacolecao->col) { // não ocorreram erros a alocar o espaço para a novacolecao
            novacolecao->length = 0;
            novacolecao->capacity = DEFAULTSIZE;
        }

        else {
            free(novacolecao);
            novacolecao = NULL;
        }
    
    }
    
    return novacolecao;

}

int colecaoIsFull(Colecao col) { // Controlo de capacidade da coleçao
    
    if (col) {
    
        return col->length == col->capacity;
    
    }

    return 1;

}

void addValueColecao(Colecao colect, void* value){ // Adiciona um valor na serriz
    
    if (colect) {
        
        if (colecaoIsFull(colect)) {
		
            colect->capacity *= 2;
            colect->col = (void**)realloc(colect->col, colect->capacity * sizeof(void*));
        
        }
	
        colect->col[colect->length] = value;
        colect->length++;
    }

}

void** getColecaoValues(Colecao colect) { return colect ? colect->col : NULL; } //Devolve Valores na serriz

unsigned long getColecaoLength(Colecao colect) { return colect ? colect->length : 0; } //Tamanho da serriz

void* getColecaoEle(Colecao col, unsigned long ind){ //Encontra um apontador pelo índice da ser

    if (col) {

        if (ind < col->length && 0 <= ind) {
        
            return col->col[ind];
        
        }

    }

    return NULL;

} 

void delColecao(Colecao colect) { //Apaga o apontador dos valores e da colecao

    if (colect) {

        free(colect->col);
		free(colect);
        colect = NULL;

    } 

}

void deepDelColecao(Colecao colect , void(*freefun)(void*)){ // //Apaga os apontadores de toda a coleçao

    if (colect) {

        for (unsigned long i = 0; i < colect->length; i++) {

            freefun(colect->col[i]); //apaga os apontadores 

        }

        free(colect->col);
        free(colect);

    }

}