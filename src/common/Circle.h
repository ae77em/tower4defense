#ifndef TP4_TOWERDEFENSE_CIRCLE_H
#define TP4_TOWERDEFENSE_CIRCLE_H

struct Circle {
    int x, y;
    int r;

    double distanceSquared(int x1, int y1, int x2, int y2) {
        int deltaX = x2 - x1;
        int deltaY = y2 - y1;
        return deltaX * deltaX + deltaY * deltaY;
    }

    bool hasCollisionWith(Circle &b){
        //Calculate total radius squared
        int totalRadiusSquared = r + b.r;
        totalRadiusSquared = totalRadiusSquared * totalRadiusSquared;

        //If the distance between the centers of the circles is less than the sum of their radii
        return (distanceSquared(x, y, b.x, b.y) < (totalRadiusSquared));
    }
};

#endif //TP4_TOWERDEFENSE_CIRCLE_H
