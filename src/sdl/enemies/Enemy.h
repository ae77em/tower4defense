#ifndef TP4_TOWERDEFENSE_SDL_ENEMY_H
#define TP4_TOWERDEFENSE_SDL_ENEMY_H

#include <vector>
#include <array>
#include "Texture.h"
#include "Point.h"
#include "Constants.h"
#include "Circle.h"
#include "Animable.h"
#include <string>

class Enemy : public Animable {
public:
    Enemy(int x, int y, SDL_Renderer *renderer, Texture *texture);
    virtual ~Enemy();

    virtual bool loadMedia();

    /*
     * Carga los sprites del monstruo en un array.
     */
    virtual void setSprites();

    void deserialize(std::string data);

    /*
     * Mueve el enemigo, siguiendo el camino indicado, y chequea colisiones.
     * */
    void move();
    void move(int maxx, int maxy);

    /*
     * Renderea la animación actual seteada para el enemigo.
     * camera: cuadrado que representa el lugar visible del mapa.
     * */
    void animate(SDL_Rect &camera);

    /*
     * Mueve al enemigo a las coordenadas (x,y) del mapa. Las coordenadas tienen
     * que indicar la posición entera dentro del mapa isométrico, teniendo en
     * cuenta que el mapa tiene columnas cartesianas.
     * */
    void moveTo(int x, int y);

    void setDirection(int d);

    void renderLifeBar(int x, int y);

    bool getIsAlive()const;

    /*
     * Modifica la textura que se utilizará para mostrar el enemigo.
     * texture: referencia a la textura a utilizar.
     * */
    void setTexture(Texture *texture);

    /*
     * Modifica la cámara del juego donde está metido el enemigo, es decir,
     * el cuadro que se encarga de mostrar la parte visible del juego.
     * camera: referencia a la cámara a utilizar.
     * */
    void setCamera(SDL_Rect &camera);

    void setRenderer(SDL_Renderer *renderer);

    const SDL_Rect &getWalkBox() const;

    Point getPoint();

    bool itIsVisible();

    void setIsAlive(bool isAlive);

    void setIsVisible(bool isVisible);

    void setEnergyPercentaje(double energyPercentaje);

private:
    /*
     * Renderea un sprite de la animación del enemigo caminando en la pantalla.
     * camera: cuadrado que representa el lugar visible del mapa.
     * */
    virtual void renderWalk(SDL_Rect &camera);

    /*
     * Renderea un sprite de la animación del enemigo muriendo en la pantalla.
     * camera: cuadrado que representa el lugar visible del mapa.
     * */
    void renderDie(SDL_Rect &camera);



protected:
    int walkingStartX = 1765;
    int walkingStartY = 3537;
    int walkSpriteWidth = 105;
    int walkSpriteHeight = 119;
    int numberOfEnemyWalkSprites = 12;
    int numberOfEnemyWalkDirections = NUMBER_OF_ENEMY_WALK_DIRECTIONS;

    int deathStartX = 1;
    int deathStartY = 2273;
    int deathSpriteWidth = 200;
    int deathSpriteHeight = 155;
    int numberOfEnemyDeathSprites = 18;
    int numberOfEnemyDeathDirections = NUMBER_OF_ENEMY_DEATH_DIRECTIONS;

    int currentDirection = 0;
    int separationBetweenSprites = 1;

    /* atributos relacionados con dibujo */
    SDL_Rect walkBox;
    std::array
            <std::array<SDL_Rect,NUMBER_OF_ENEMY_WALK_SPRITES>,
                    NUMBER_OF_ENEMY_WALK_DIRECTIONS> walkingSprites;

    SDL_Rect deathBox;
    std::array
            <std::array<SDL_Rect, NUMBER_OF_ENEMY_DEATH_SPRITES>,
                    NUMBER_OF_ENEMY_DEATH_DIRECTIONS> deathSprites;

    bool firstFrameOfDeathRendered = false;
    bool lastFrameOfDeathRendered = false;

    Circle collisionCircle;

    int velocityX;
    int velocityY;

    SDL_Renderer *renderer;
    Texture *texture = new Texture();

    Point currentPoint;

    bool isAlive;
    float energyPercentaje;
    bool isVisible;

    static const int LIFE_BAR_WIDTH = 50;
    static const int LIFE_BAR_HEIGHT = 4;

    virtual void initializeSpritesData();

};


#endif //TP4_TOWERDEFENSE_ENEMY_H
