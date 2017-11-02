#ifndef TP4_TOWERDEFENSE_UTILS_H
#define TP4_TOWERDEFENSE_UTILS_H

#include <SDL2/SDL_rect.h>
#include "../common/Point.h"

class Utils {
public:
    static bool checkCollision(SDL_Rect a, SDL_Rect b);

    static Point mapToScreen(int i, int j, int h_offset = 0, int w_offset = 0);
    static Point screenToMap(int x, int y);
    static Point getMouseRelativePoint(const SDL_Rect &camera);

    static SDL_Rect getBoxByTileType(int type);

    static DecimalPoint mapToScreenDecimal(double i, double j);

    static DecimalPoint twoDimToIso(double x, double y);

    DecimalPoint isoToCartesian(double isoX, double isoY);

    static DecimalPoint cartesianToIso(double carX, double carY);
};


#endif //TP4_TOWERDEFENSE_UTILS_H
