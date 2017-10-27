#include "Mapa.h"
#include <string>

Mapa::Mapa(unsigned x, unsigned y) :extension_x(x), extension_y(y) {
    Terreno pradera('p');
    casillas = std::vector(x * y, pradera);
}
