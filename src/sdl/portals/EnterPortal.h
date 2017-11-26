#ifndef TP4_TOWERDEFENSE_PORTAL_H
#define TP4_TOWERDEFENSE_PORTAL_H


#include <SDL2/SDL_render.h>
#include "../Animable.h"
#include "../Texture.h"
#include "../../common/Circle.h"

class EnterPortal : public Animable {
public:
    EnterPortal(int x, int y, SDL_Renderer *renderer, Texture *texture);
    virtual ~EnterPortal();

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
    int blueSpriteWidth = 136;
    int blueSpriteHeight = 181;
    int numberOfPortalSprites = 30;

    /* atributos relacionados con dibujo */
    SDL_Rect drawBox;

    SDL_Renderer *renderer;
    Texture *texture = new Texture();

    Point currentPoint;

    SDL_Rect spriteClipsPortalBlue[30];
};


#endif //TP4_TOWERDEFENSE_PORTAL_H
