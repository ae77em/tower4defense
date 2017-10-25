#include <vector>
#include <utility>

#ifndef JUEGO_H
#define JUEGO_H
#define T Juego

/* -----> x
 * |
 * v
 * y
 */
class T {
    std::vector<std::pair<unsigned, unsigned>> torres;

public:
    void agregarTorre(unsigned x, unsigned y);
};

#undef T
#endif
