#include <SDL2/SDL_mouse.h>
#include "SdlUtils.h"
#include "../sdl/Constants.h"
#include "Animable.h"
#include "../common/Protocol.h"
#include <stdexcept>
#include <cmath>

Point SdlUtils::mapToScreen(int i, int j, int h_offset, int w_offset) {
    int x = (i - j) * (ISO_TILE_WIDTH + w_offset) / 2;
    int y = (i + j) * (ISO_TILE_HEIGHT + h_offset) / 2;

    return Point(x, y);
}

DecimalPoint SdlUtils::mapToScreenDecimal(double i, double j) {
    double x = (i - j) * (double(ISO_TILE_WIDTH)) / double(2);
    double y = (i + j) * (double(ISO_TILE_HEIGHT)) / double(2);

    return DecimalPoint(x, y);
}

Point SdlUtils::screenToMap(int x, int y) {
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

DecimalPoint SdlUtils::screenToMapDecimal(int x, int y) {
    double x_d = static_cast<double>  (x);
    double y_d = static_cast<double>  (y);

    double yByIsoTileHeightHalf = y_d / ISO_TILE_HEIGHT_HALF;
    double xByIsoTileWithHalf = x_d / ISO_TILE_WIDTH_HALF;

    double i = (yByIsoTileHeightHalf + xByIsoTileWithHalf) / 2;
    double j = (yByIsoTileHeightHalf - xByIsoTileWithHalf) / 2;

    j += 0.5;

    return DecimalPoint(i, j);
}


DecimalPoint SdlUtils::twoDimToIso(double x, double y) {
    double yByIsoTileHeightHalf = y / ISO_TILE_HEIGHT_HALF;
    double xByIsoTileWithHalf = x / ISO_TILE_WIDTH_HALF;

    double i = (xByIsoTileWithHalf + yByIsoTileHeightHalf);
    double j = (xByIsoTileWithHalf - yByIsoTileHeightHalf) / 2;

    return DecimalPoint(i, j);
}

DecimalPoint SdlUtils::isoToCartesian(double isoX, double isoY) {
    double carX = (isoX - isoY) / 1.5;
    double carY = isoX / 3.0 + isoY / 1.5;

    return DecimalPoint(carX, carY);
}

DecimalPoint SdlUtils::cartesianToIso(double carX, double carY) {
    double isoX = carX - carY;
    double isoY = (carY + carX) / 2.0;

    return DecimalPoint(isoX, isoY);
}

Point SdlUtils::getMouseRelativePoint(const SDL_Rect &camera) {
    int mousePosX, mousePosY;

    SDL_GetMouseState(&mousePosX, &mousePosY);

    mousePosX += camera.x;
    mousePosY += camera.y;

    Point point = SdlUtils::screenToMap(mousePosX, mousePosY);

    return point;
}

SDL_Rect SdlUtils::getBoxByTileType(int type) {
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

Point SdlUtils::findClosest(const Point& p, const std::vector<Point>& l) {
    if (l.size() == 0)
        throw std::runtime_error("passed 0 sized vector to findClosest");

    /* Primera aproximacion al minimo, l[0] */
    auto min = l.begin();
    int d_sqr = pow((p.x - min->x), 2) + pow((p.y - min->y), 2);

    auto end = l.end();
    for (auto it = l.begin(); it != end; it++) {
        int curr_d = pow((p.x - it->x), 2) + pow((p.y - it->y), 2);

        /* Si es una mejor aproximacion, actualizar */
        if (curr_d < d_sqr) {
            min = it;
            d_sqr = curr_d;
        }
    }

    return *min;
}
