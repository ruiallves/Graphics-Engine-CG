#ifndef CP
#define CP
#include <stdlib.h>

#define DEFAULTSIZE 1000

typedef struct colecao* Colecao;

Colecao newEmptyColecao();

int ColecaoIsFull(Colecao);

void addValueColecao(Colecao, void*);

void** getColecaoValues(Colecao);

unsigned long getColecaoLength(Colecao);

void* getColecaoEle(Colecao, unsigned long);

void delColecao(Colecao);

void deepDelColecao(Colecao, void (*)(void*));

#endif // colecao de Pontos