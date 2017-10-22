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

Point Utils::mapToScreen(int i, int j) {
    int x = (i - j) * ISO_TILE_WIDTH / 2;
    int y = (i + j) * ISO_TILE_HEIGHT / 2;

    return Point(x,y);
}

Point Utils::screenToMap(int x, int y) {

    std::cout
            << "mouse x: "
            << std::to_string(x)
            << "; mouse y: "
            << std::to_string(y);

    int i = (x / (ISO_TILE_WIDTH / 2) + y / (ISO_TILE_HEIGHT / 2)) / 2;
    int j = (y / (ISO_TILE_HEIGHT / 2) - (x / (ISO_TILE_WIDTH / 2))) /2;

    std::cout
            << "-> screen i: "
            << std::to_string(i)
            << "; screen j: "
            << std::to_string(j)
            << std::endl;

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

