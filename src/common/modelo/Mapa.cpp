#include "Mapa.h"
#include "../Point.h"
#include <string>
#include <vector>
#include <stdexcept>

Mapa::Mapa(unsigned x, unsigned y) :extension_x(x), extension_y(y),
        casillas(x * y, '.'), caminos() {}

char Mapa::casilla(unsigned x, unsigned y) {
    if (x >= extension_x || y >= extension_y)
        throw std::runtime_error("out of bounds access: " + std::to_string(x)
                + "," + std::to_string(y) + " on "
                + std::to_string(extension_x) + " x "
                + std::to_string(extension_y) + " map");
    return casillas[x + y * extension_x];
}

Point Mapa::dimensiones() {
    return Point(extension_x, extension_y);
}

void Mapa::setCasilla(char value, unsigned x, unsigned y) {
    if (x >= extension_x || y >= extension_y)
        throw std::runtime_error("out of bounds access: " + std::to_string(x)
                + "," + std::to_string(y) + " on "
                + std::to_string(extension_x) + " x "
                + std::to_string(extension_y) + " map");
    casillas[x + y * extension_x] = value;
}

std::vector<std::vector<Point>>& Mapa::getCaminos() {
    return caminos;
}

void Mapa::agregarCamino(const std::vector<Point> &camino) {
    caminos.push_back(camino);
}
