#include <SDL2/SDL_mouse.h>
#include "Utils.h"
#include "../sdl/Constants.h"
#include "Animable.h"

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
    int x = (i - j) * (ISO_TILE_WIDTH + w_offset) / 2;
    int y = (i + j) * (ISO_TILE_HEIGHT + h_offset) / 2;

    return Point(x, y);
}

DecimalPoint Utils::mapToScreenDecimal(double i, double j) {
    double x = (i - j) * (double(ISO_TILE_WIDTH)) / double(2);
    double y = (i + j) * (double(ISO_TILE_HEIGHT)) / double(2);

    return DecimalPoint(x, y);
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

DecimalPoint Utils::screenToMapDecimal(int x, int y) {
    double x_d = static_cast<double>  (x);
    double y_d = static_cast<double>  (y);

    double yByIsoTileHeightHalf = y_d / ISO_TILE_HEIGHT_HALF;
    double xByIsoTileWithHalf = x_d / ISO_TILE_WIDTH_HALF;

    double i = (yByIsoTileHeightHalf + xByIsoTileWithHalf) / 2;
    double j = (yByIsoTileHeightHalf - xByIsoTileWithHalf) / 2;

    //double i = d_i - 0.5;
    j += 0.5;

    return DecimalPoint(i, j);
}


DecimalPoint Utils::twoDimToIso(double x, double y) {
    double yByIsoTileHeightHalf = y / ISO_TILE_HEIGHT_HALF;
    double xByIsoTileWithHalf = x / ISO_TILE_WIDTH_HALF;

    double i = (xByIsoTileWithHalf + yByIsoTileHeightHalf);
    double j = (xByIsoTileWithHalf - yByIsoTileHeightHalf) / 2;

    return DecimalPoint(i, j);
}

DecimalPoint Utils::isoToCartesian(double isoX, double isoY) {
    double carX = (isoX - isoY) / 1.5;
    double carY = isoX / 3.0 + isoY / 1.5;

    return DecimalPoint(carX, carY);
}

DecimalPoint Utils::cartesianToIso(double carX, double carY) {
    double isoX = carX - carY;
    double isoY = (carY + carX) / 2.0;

    return DecimalPoint(isoX, isoY);
}

Point Utils::getMouseRelativePoint(const SDL_Rect &camera) {
    int mousePosX, mousePosY;

    SDL_GetMouseState(&mousePosX, &mousePosY);

    mousePosX += camera.x;
    mousePosY += camera.y;

    Point point = Utils::screenToMap(mousePosX, mousePosY);

    return point;
}

SDL_Rect Utils::getBoxByTileType(int type) {
    SDL_Rect toReturn;

    switch (type) {
        case static_cast<int>(TileType::TILE_GRASS):
        case static_cast<int>(TileType::TILE_DESERT):
        case static_cast<int>(TileType::TILE_ICE):
        case static_cast<int>(TileType::TILE_LAVA):
        case static_cast<int>(TileType::TILE_FIRM):
        case static_cast<int>(TileType::TILE_WAY):
        case static_cast<int>(TileType::TILE_FIRM_MARKED):
            toReturn.x = 0;
            toReturn.x = 0;
            toReturn.y = 0;
            toReturn.h = ISO_TILE_HEIGHT;
            toReturn.w = ISO_TILE_WIDTH;
            break;
        default:
            toReturn.x = 0;
            toReturn.y = 0;
            toReturn.h = ISO_TILE_HEIGHT;
            toReturn.w = ISO_TILE_WIDTH;
    }

    return toReturn;
}

double Utils::distanceSquared(int x1, int y1, int x2, int y2) {
    int deltaX = x2 - x1;
    int deltaY = y2 - y1;
    return deltaX * deltaX + deltaY * deltaY;
}

bool Utils::hasCircleCollision(Circle &a, Circle &b) {
    //Calculate total radius squared
    int totalRadiusSquared = a.r + b.r;
    totalRadiusSquared = totalRadiusSquared * totalRadiusSquared;

    //If the distance between the centers of the circles is less than
    // the sum of their radii
    return (distanceSquared(a.x, a.y, b.x, b.y) < (totalRadiusSquared));
}

bool Utils::animablesPositionComparator(Animable *a, Animable *b) {
    bool toReturn;
    Point A(0,0), B(0,0);
    /* si por algún motivo falla... */
    if (a){
        A = a->getPoint();
    } else {
        return false;
    }
    if (b){
        B = b->getPoint();
    } else {
        return true;
    }

    if (A.x < B.x){
        toReturn = true;
    } else if (A.x > B.x){
        toReturn = false;
    } else {
        if (A.y <= B.y){
            toReturn = true;
        } else {
            toReturn = false;
        }
    }

    return toReturn;
}
