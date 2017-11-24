#ifndef TP4_TOWERDEFENSE_CIRCLE_H
#define TP4_TOWERDEFENSE_CIRCLE_H

#include <cmath>

struct Circle {
    int x, y;
    int r;

    //FIXME: duplicate of Utils::distanceSquared
    double distanceSquared(int x1, int y1, int x2, int y2) {
        int deltaX = x2 - x1; int deltaY = y2 - y1;
        return deltaX * deltaX + deltaY * deltaY;
    }

    /* Circles overlap iff distance between centers is less than sum of radii
     *
     * Due to the monotonicity of polynomials, comparing the squares of
     * these quantities is equivalent to comparing the quantities themselves.
     * (proof of this theorem sold separately)
     */
    bool hasCollisionWith(Circle &b){
        /* Square of the sum of the radii */
        int r2 = std::pow(r + b.r, 2);

        return (distanceSquared(x, y, b.x, b.y) < r2);
    }
};

#endif //TP4_TOWERDEFENSE_CIRCLE_H
