#ifndef TP4_TOWERDEFENSE_EXITPORTAL_H
#define TP4_TOWERDEFENSE_EXITPORTAL_H

#include <SDL2/SDL_render.h>
#include "Animable.h"
#include "Texture.h"
#include "Circle.h"

class ExitPortal : public Animable {
public:
    ExitPortal(int x, int y, SDL_Renderer *renderer, Texture *texture);
    virtual ~ExitPortal();

    /*
     * Carga los sprites del monstruo en un array.
     */
    virtual void setSprites();

    /*
     * Renderea la animación actual seteada para el enemigo.
     * camera: cuadrado que representa el lugar visible del mapa.
     * */
    void animate(SDL_Rect &camera);

    Point getPoint();

private:
    /*
     * Renderea un sprite de la animación del enemigo caminando en la pantalla.
     * camera: cuadrado que representa el lugar visible del mapa.
     * */
    virtual void render(SDL_Rect &camera);

protected:
    int redSpriteWidth = 136;
    int redSpriteHeight = 185;
    int numberOfPortalSprites = 30;

    /* atributos relacionados con dibujo */
    SDL_Rect drawBox;

    SDL_Renderer *renderer;
    Texture *texture = new Texture();

    Point currentPoint;

    SDL_Rect spriteClipsPortalRed[30];
};

#endif //TP4_TOWERDEFENSE_EXITPORTAL_H
