#ifndef TP4_TOWERDEFENSE_AIR_H
#define TP4_TOWERDEFENSE_AIR_H

#include <vector>
#include "../Texture.h"
#include "../../common/Point.h"
#include "../Constants.h"
#include "../../common/Circle.h"
#include "../Animable.h"
#include "Tower.h"
#include <string>

class Air : public Tower {
public:
    Air(int x, int y, SDL_Renderer *renderer, Texture &t);

    ~Air();

    /*
     * Setea la posición de la torre en la pantalla.
     */
    void setPosition(int x, int y);

    /*
     * Carga los sprites de la torre en un array.
     */
    void setSprites();

    /*
     * Renderea un sprite de la animación de la torre esperando.
     * camera: cuadrado que representa el lugar visible del mapa.
     * */
    void renderIdle(SDL_Rect &camera);

    /*
     * Renderea un sprite de la animación de la torre disparando.
     * camera: cuadrado que representa el lugar visible del mapa.
     * */
    void renderShot(SDL_Rect &camera);

    /*
     * Renderea la animación actual seteada para la torre.
     * camera: cuadrado que representa el lugar visible del mapa.
     * */
    void animate(SDL_Rect &camera);

    const SDL_Rect &getIdleBox() const;

    Point getPoint();

    void shiftColliders();

    Circle &getCollisionCircle();

    void setIsShooting(bool isShooting);

protected:
    static const int NUMBER_OF_IDLE_SPRITES = 17;
    static const int NUMBER_OF_SHOT_SPRITES = 17;

    const int IDLE_SPRITE_WIDTH = 57;
    const int IDLE_SPRITE_HEIGHT = 99;

    const int SHOT_SPRITE_WIDTH = 57;
    const int SHOT_SPRITE_HEIGHT = 99;

    const int separationBetweenSprites = 1;

    SDL_Rect idleBox;
    SDL_Rect shotBox;

    Texture &texture;
    SDL_Renderer *renderer;
    SDL_Rect idleSprites[NUMBER_OF_IDLE_SPRITES];
    SDL_Rect shotSprites[NUMBER_OF_SHOT_SPRITES];

    int idleSpriteWidth;
    int idleSpriteHeight;

    int shotSpriteWidth;
    int shotSpriteHeight;

    int numberOfIdleSprites;
    int numberOfShotSprites;

    // seteo los sprites para espera...
    int idleStartX = 2;
    int idleStartY = 568;
    int shotStartX = 2;
    int shotStartY = 18;

    Point currentPoint;
    Circle collisionCircle;

    bool isShooting;
};

#endif //TP4_TOWERDEFENSE_AIR_H
