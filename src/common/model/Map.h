#ifndef MAPA_H
#define MAPA_H

#include <string>
#include <vector>
#include "Point.h"
#include "Enemy.h"
#include <tuple>

namespace model {

/* entry = enemy_type, horde_size, path_index, delay_in_seconds */
typedef std::tuple<std::string, int, int, int> entry;

/* Representa el terrento del juego, independiente de su representacion
   visual o de las criaturas que se encuentren en el. */
class Map {
    unsigned extension_x, extension_y;
    std::vector<char> tiles;
    std::vector<std::vector<Point>> paths;
    std::vector<entry> hordas;
    char background_style;
    std::string name;

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

    const std::vector<entry>& getHordes() const;
    void addHorde(const std::string& enemy_type, int horde_size,
            int path_index, int delay_seconds);

    std::string &getName();
    void setName(std::string aName);

    static Map loadFromString(std::string json);
    static Map loadFromFile(std::string filename);

    void checkValid();
};

} // namespace model
#endif
