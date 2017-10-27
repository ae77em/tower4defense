#ifndef MAPA_H
#define MAPA_H

#include <string>

class Mapa {
    unsigned extension_x, extension_y;
    std::vector<Terreno> casillas;

public:
    Mapa(unsigned x, unsigned y);
}

#endif
