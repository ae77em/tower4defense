#include <SDL_mouse.h>
#include "Utils.h"
#include "Constants.h"

#include <iostream>

bool Utils::checkCollision(SDL_Rect a, SDL_Rect b) {
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if (bottomA <= topB) {
        return false;
    }

    if (topA >= bottomB) {
        return false;
    }

    if (rightA <= leftB) {
        return false;
    }

    if (leftA >= rightB) {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

Point Utils::mapToScreen(int i, int j, int h_offset, int w_offset) {
    int x = (i - j) * (ISO_TILE_WIDTH + w_offset)/ 2;
    int y = (i + j) * (ISO_TILE_HEIGHT + h_offset) / 2;

    return Point(x,y);
}

Point Utils::screenToMap(int x, int y) {
    double x_d = static_cast<double>  (x);
    double y_d = static_cast<double>  (y);

    double yByIsoTileHeightHalf = y_d / ISO_TILE_HEIGHT_HALF;
    double xByIsoTileWithHalf = x_d / ISO_TILE_WIDTH_HALF;

    double d_i = (yByIsoTileHeightHalf + xByIsoTileWithHalf) / 2;
    double d_j = (yByIsoTileHeightHalf - xByIsoTileWithHalf) / 2;

    int i = int(d_i + 0.5) - 1;
    int j = int(d_j + 0.5);

    return Point(i, j);
}

Point Utils::getMouseRelativePoint(const SDL_Rect &camera) {
    int mousePosX, mousePosY;

    SDL_GetMouseState(&mousePosX, &mousePosY);

    mousePosX += camera.x;
    mousePosY += camera.y;

    Point point = Utils::screenToMap(mousePosX,
                                     mousePosY);

    return point;
}

