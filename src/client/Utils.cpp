#include "Utils.h"
#include "Constants.h"

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

Point Utils::map_to_screen(int i, int j) {
    int x = (i - j) * ISO_TILE_WIDTH / 2;
    int y = (i + j) * ISO_TILE_HEIGHT / 2;

    return Point(x,y);
}

Point Utils::screen_to_map(int x, int y) {
    int i = (x / (ISO_TILE_WIDTH / 2) + y / (ISO_TILE_HEIGHT / 2)) /2;
    int j = (y / (ISO_TILE_HEIGHT / 2) - (x / (ISO_TILE_WIDTH / 2))) /2;

    return Point(i, j);
}
