#ifndef TP4_TOWERDEFENSE_WATER_H
#define TP4_TOWERDEFENSE_WATER_H


#include "Tower.h"
class Water : public Tower {
public:
    Water(int x, int y, SDL_Renderer *renderer, Texture &t);

    ~Water();

    /*
     * Setea la posición de la torre en la pantalla.
     */
    void setPosition(int x, int y);

    /*
     * Carga los sprites de la torre en un array.
     */
    void setSprites();

    /*
     * Toma los datos de configuración de la torre de un archivo y lo nutre
     * con eso.
     */
    void deserialize(std::string data);

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
    static const int NUMBER_OF_IDLE_SPRITES = 13;
    static const int NUMBER_OF_SHOT_SPRITES = 13;

    const int IDLE_SPRITE_WIDTH = 56;
    const int IDLE_SPRITE_HEIGHT = 116;

    const int SHOT_SPRITE_WIDTH = 56;
    const int SHOT_SPRITE_HEIGHT = 116;

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
    int idleStartX = 1;
    int idleStartY = 17;
    int shotStartX = 1;
    int shotStartY = 209;

    Point currentPoint;
    Circle collisionCircle;

    bool isShooting;
};

#endif //TP4_TOWERDEFENSE_WATER_H
