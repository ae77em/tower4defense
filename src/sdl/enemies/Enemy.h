#ifndef TP4_TOWERDEFENSE_SDL_ENEMY_H
#define TP4_TOWERDEFENSE_SDL_ENEMY_H

static const int LIFE_BAR_WIDTH = 50;

static const int LIFE_BAR_HEIGHT = 4;

#include "../../common/model/Enemy.h"
#include <vector>
#include <array>
#include "../Texture.h"
#include "../../common/Point.h"
#include "../Constants.h"
#include "../../common/Circle.h"
#include "../Animable.h"
#include <string>

class Enemy : public Animable {
public:
    Enemy(int x, int y, SDL_Renderer *renderer, Texture *texture);

    virtual ~Enemy();

    /*
     * Carga la imagen que contiene los sprites del monstruo.
     */
    virtual bool loadMedia();

    /*
     * Carga los sprites del monstruo en un array.
     */
    virtual void setSprites();

    /*
     * Toma los datos de configuración del enemigo de un archivo y lo nutre
     * con eso.
     */
    void deserialize(std::string data);

    /*
     * Mueve el enemigo, siguiendo el camino indicado, y chequea colisiones.
     * */
    void move();
    void move(int maxx, int maxy);

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

    /*
     * Renderea la animación actual seteada para el enemigo.
     * camera: cuadrado que representa el lugar visible del mapa.
     * */
    void animate(SDL_Rect &camera);

    void kill();

    /*
     * Mueve al enemigo a las coordenadas (x,y) del mapa. Las coordenadas tienen
     * que indicar la posición entera dentro del mapa isométrico, teniendo en
     * cuenta que el mapa tiene columnas cartesianas.
     *
     * */
    void moveTo(int x, int y);

    void setDirection(int d);

    void renderLifeBar(int x, int y);

    bool itIsAlive()const;

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

    /*
     * Setea el renderer, encargad de dibujar la textura del enemigo en el juego.
     * camera: puntero al renderer utilizado.
     * */
    void setRenderer(SDL_Renderer *renderer);

    /*
     * Setea la velocidad
     * camera: puntero al renderer utilizado.
     * */
    void setVelocity(int velocityX, int velocityY);

    /*
     * Retirba el bonus que se entrega por matar a este enemigo.
     * Corresponde al 50% de los puntos de vida del monstruo.
     * */
    int getBonus();

    const SDL_Rect &getWalkBox() const;

    Circle &getCollisionCircle();


    static int getCollisionCircleRadio();

    const std::vector<Point> &getPath() const;

    void setPath(const std::vector<Point> &path);

    void shiftColliders();

    Point getPoint();

    bool itIsVisible();

    void setIsVisible(bool isVisible);

    void setEnergyPercentaje(double energyPercentaje);

protected:
    /* Point for get the sprites */
    int walkingStartX = 1765;
    int walkingStartY = 3537;
    int walkSpriteWidth = 105;
    int walkSpriteHeight = 119;
    int numberOfEnemyWalkSprites = NUMBER_OF_ENEMY_WALK_SPRITES;
    int numberOfEnemyWalkDirections = NUMBER_OF_ENEMY_WALK_DIRECTIONS;

    int deathStartX = 1;
    int deathStartY = 2273;
    int deathSpriteWidth = 200;
    int deathSpriteHeight = 155;
    int numberOfEnemyDeathSprites = NUMBER_OF_ENEMY_DEATH_SPRITES;
    int numberOfEnemyDeathDirections = NUMBER_OF_ENEMY_DEATH_DIRECTIONS;

    int currentDirection = 0;
    int separationBetweenSprites = 1;

    /* ATRIBUTOS RELACIONADOS CON DIBUJO */
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

protected:

    /* METHODS */
    virtual void initializeSpritesData();
};


#endif //TP4_TOWERDEFENSE_ENEMY_H
