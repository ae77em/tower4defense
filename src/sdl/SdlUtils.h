#ifndef TP4_TOWERDEFENSE_UTILS_H
#define TP4_TOWERDEFENSE_UTILS_H

#include <SDL2/SDL_rect.h>
#include "Point.h"

class SdlUtils {
public:

    static Point
    mapToScreen(int i, int j, int h_offset = 0, int w_offset = 0);

    /*
     * Verifica a qué coordenada (x,y) del mapa pertenece un punto de la
     * pantalla clickeado, teniendo usando como punto el cuadrado que
     * corresponde al tile.
     * Por ejemplo: el punto (0,0) corresponde al tile en dicha posición en el
     * mapa de tiles.
     */
    static Point screenToMap(int x, int y);

    /*
     * Retorna el punto dentro del lienzo general (el que contiene el total del mapa)
     * donde se hizo click. Para ello toma la posición del click, correspondiente
     * a la posición en el cuadro de la cámara, y le suma le offset de la cámara en
     * el conjunto del mapa.
     */
    static Point getMouseRelativePoint(const SDL_Rect &camera);

    static SDL_Rect getBoxByTileType(int type);

    static DecimalPoint mapToScreenDecimal(double i, double j);

    static DecimalPoint twoDimToIso(double x, double y);

    DecimalPoint isoToCartesian(double isoX, double isoY);

    static DecimalPoint cartesianToIso(double carX, double carY);

    static DecimalPoint screenToMapDecimal(int x, int y);

    /* Devuelve un punto en l a distancia minimal de p
     *
     * Pasar un l vacio resulta en error en tiempo de ejecucion.
     */
    static Point findClosest(const Point &p, const std::vector<Point> &l);
};

#endif //TP4_TOWERDEFENSE_UTILS_H
