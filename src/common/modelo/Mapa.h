#ifndef MAPA_H
#define MAPA_H

#include <string>
#include <vector>
#include "../Point.h"
#include "Enemy.h"

namespace model {

/* Representa el terrento del juego, independiente de su representacion
   visual o de las criaturas que se encuentren en el. */
class Mapa {
    unsigned extension_x, extension_y;
    std::vector<char> casillas;
    std::vector<std::vector<Point>> caminos;
    std::vector<std::vector<model::Enemy>> enemigos;
    char estilo_fondo;
    std::string nombre;

public:
    Mapa(); /* para poder usar el from string */
    Mapa(unsigned x, unsigned y);
    explicit Mapa(std::string filename);
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
    char casilla(unsigned x, unsigned y);
    void setCasilla(char value, unsigned x, unsigned y);

    /* Devuelve el estilo de las casillas de espacio transitable.
       d desert
       g grass
       i ice
       l lava
     */

    char getEstiloFondo();
    void setEstiloFondo(char estilo);

    Point dimensiones();
    /* Devuelve verdadero si el parametro esta dentro de los bordes */
    bool estaDentro(Point &p) const;

    std::vector<std::vector<Point>>& getCaminos();
    void agregarCamino(const std::vector<Point> &camino);
    std::vector<std::vector<model::Enemy>>& getEnemigos();
    void agregarEnemigo(int indice_camino, const Enemy &e);
    std::string &getNombre();
    void setNombre(std::string un_nombre);

    void cargarDesdeString(std::string json);
};

} // namespace model
#endif
