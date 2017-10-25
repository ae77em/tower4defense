#include "Juego.h"

#define T Juego

void T::agregarTorre(unsigned x, unsigned y) {
    torres.emplace_back(x, y);
}

