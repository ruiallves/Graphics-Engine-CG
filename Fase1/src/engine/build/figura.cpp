#include <cstring>
#include <vector>
#include "figura.h"

Figura newFigura() {
    Figura novaFigura = new struct figura;
    novaFigura->nome = nullptr; 
    return novaFigura;
}
