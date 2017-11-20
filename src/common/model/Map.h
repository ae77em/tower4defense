#ifndef MAPA_H
#define MAPA_H

#include <string>
#include <vector>
#include "../Point.h"
#include "Enemy.h"

namespace model {

/* Representa el terrento del juego, independiente de su representacion
   visual o de las criaturas que se encuentren en el. */
class Map {
    unsigned extension_x, extension_y;
    std::vector<char> tiles;
    std::vector<std::vector<Point>> paths;
    std::vector<
        std::pair<int, std::vector<std::string>>
    > hordas;
    char background_style;
    std::string name;
    int delay_hordes_seg; // El tiempo en segundos entre las hordas

public:
    Map();
    Map(unsigned x, unsigned y);
    std::string serialize();

    /* Devuelve el contenido de la casilla en la posicion x, y.
       Lanza una excepcion si x,y no estan dentro del rango.
       Las coordenadas comienzan en la posicion 0,0

       Tabla de codigos:
       . espacio transitable
       # espacio vacio/impasable
       x terreno firme (construccion de torres posible)
       Torres
            ~ agua
            @ aire
            ! fuego
            * tierra
       Portales
            E entrada
            S salida
     */
    char tile(unsigned x, unsigned y);
    void setTile(char value, unsigned x, unsigned y);

    /* Devuelve el estilo de las casillas de espacio transitable.
       d desert
       g grass
       i ice
       l lava
     */
    char getBackgroundStyle();
    void setBackgroundStyle(char estilo);

    Point dimensions();
    /* Devuelve verdadero si el parametro esta dentro de los bordes */
    bool isIn(Point &p) const;

    std::vector<std::vector<Point>>& getPaths();
    void addPaths(const std::vector<Point> &camino);

    const std::vector<std::pair<int, std::vector<std::string>>>& getHordes();
    void addHorde(int path, std::vector<std::string> enemies);
    int getDelay() const;
    void setDelay(int delay);

    std::string &getName();
    void setName(std::string aName);

    static Map loadFromString(std::string json);
    static Map loadFromFile(std::string filename);
};

} // namespace model
#endif
