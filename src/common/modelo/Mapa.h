#ifndef MAPA_H
#define MAPA_H

#include <string>
#include <vector>
#include "../Point.h"

/* Representa el terrento del juego, independiente de su representacion
 * visual o de las criaturas que se encuentren en el.
 */

class Mapa {
    unsigned extension_x, extension_y;
    std::vector<char> casillas;

public:
    Mapa(unsigned x, unsigned y);

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
    Point dimensiones();
};

#endif
