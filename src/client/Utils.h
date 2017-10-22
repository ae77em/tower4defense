//
// Created by esteban on 21/10/17.
//

#ifndef TP4_TOWERDEFENSE_UTILS_H
#define TP4_TOWERDEFENSE_UTILS_H


#include <SDL_rect.h>
#include "Point.h"

class Utils {
public:
    static bool checkCollision(SDL_Rect a, SDL_Rect b);
    static Point mapToScreen(int i, int j, int h_offset = 0, int w_offset = 0);
    static Point screenToMap(int x, int y);
    static Point getMouseRelativePoint(const SDL_Rect &camera);
};


#endif //TP4_TOWERDEFENSE_UTILS_H
