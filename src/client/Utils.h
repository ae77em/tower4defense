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
    static Point map_to_screen(int i, int j);
    static Point screen_to_map(int x, int y);
};


#endif //TP4_TOWERDEFENSE_UTILS_H
