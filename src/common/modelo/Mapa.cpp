#include "Mapa.h"
#include <string>
#include <vector>
#include <stdexcept>

Mapa::Mapa(unsigned x, unsigned y) :extension_x(x), extension_y(y) {
    casillas = std::vector<char>(x * y, '.');
}

char Mapa::casilla(unsigned x, unsigned y) {
    if (x >= extension_x || y >= extension_y)
        throw std::runtime_error("out of bounds access: " + std::to_string(x)
                + "," + std::to_string(y) + " on "
                + std::to_string(extension_x) + " x "
                + std::to_string(extension_y) + " map");
    return casillas[x + y * extension_x];
}
