#ifndef TP4_TOWERDEFENSE_CIRCLE_H
#define TP4_TOWERDEFENSE_CIRCLE_H

#include <cmath>

struct Circle {
    int x, y;
    int r;

    double distanceSquared(int x1, int y1, int x2, int y2) {
        int deltaX = x2 - x1;
        int deltaY = y2 - y1;
        return deltaX * deltaX + deltaY * deltaY;
    }

    /* Los círculos se superponen si la distancia entre centros es menor que
     * la suma de radios.
     *
     * Debido a la monotonicidad de los polinomios, comparando los cuadrados de
     * estas cantidades equivalen a comparar las cantidades mismas.
     * (prueba de este teorema se vende por separado).
     */
    bool hasCollisionWith(Circle &b){
        /* Square of the sum of the radii */
        double r2 = std::pow(r + b.r, 2);
        return (distanceSquared(x, y, b.x, b.y) < r2);
    }
};

#endif //TP4_TOWERDEFENSE_CIRCLE_H
