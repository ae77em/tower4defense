#ifndef TP4_TOWERDEFENSE_SDL_ENEMY_H
#define TP4_TOWERDEFENSE_SDL_ENEMY_H


#include <vector>
#include <array>
#include "../LTexture.h"
#include "../../common/Point.h"
#include "../Constants.h"
#include "../Circle.h"

class Enemy {
public:
    Enemy(int x, int y, SDL_Renderer *renderer, LTexture *texture);

    ~Enemy();

    /*
     * Carga la imagen que contiene los sprites del monstruo.
     */
    virtual bool loadMedia();

    /*
     * Setea la posición del monstruo en la pantalla.
     */
    void setPosition(int x, int y);

    /*
     * Carga los sprites del monstruo en un array.
     */
    void setSprites();

    /*
     * Toma los datos de configuración del enemigo de un archivo y lo nutre
     * con eso.
     */
    void deserialize(std::string data);

    /*
     * Mueve el enemigo, siguiendo el camino indicado, y chequea colisiones.
     * */
    void move();

    /*
     * Renderea un sprite de la animación del enemigo caminando en la pantalla.
     * camera: cuadrado que representa el lugar visible del mapa.
     * */
    void renderWalk(SDL_Rect &camera);

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

    /*
     * Setea la propiedad isAlive en false del enemigo.
     * */
    void kill();

    /*
     * Mueve al enemigo a las coordenadas (x,y) del mapa. Las coordenadas tienen
     * que indicar la posición entera dentro del mapa isométrico, teniendo en cuenta
     * que el mapa tiene columnas cartesianas.
     *
     * */
    void moveTo(int x, int y);

    void setDirection(int d);

    void renderLifeBar(int x, int y);

    bool itIsAlive()const;

    int getLifePoints() const;

    void quitLifePoints(int points);

    int getVelocity() const;

    int getIsAir() const;
    /*
     * Modifica la textura que se utilizará para mostrar el enemigo.
     * texture: referencia a la textura a utilizar.
     * */
    void setTexture(LTexture *texture);

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

protected:
    // Defino este monstruo por default...
    const std::string TEXTURE_FILE_PATH = "images/sprites/enemy-abominable-walk.png";

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

    const int MAX_VELOCITY = 10;

    bool isAlive;
    int initialLifePoints;
    int remainingLifePoints;
    int velocity;
    int isAir;

    /* ATRIBUTOS RELACIONADOS CON DIBUJO */
    SDL_Rect walkBox;
    std::array<std::array<SDL_Rect,NUMBER_OF_ENEMY_WALK_SPRITES>, NUMBER_OF_ENEMY_WALK_DIRECTIONS> walkingSprites;

    SDL_Rect deathBox;
    std::array<std::array<SDL_Rect, NUMBER_OF_ENEMY_DEATH_SPRITES>, NUMBER_OF_ENEMY_DEATH_DIRECTIONS> deathSprites;

    bool firstFrameOfDeathRendered = false;
    bool lastFrameOfDeathRendered = false;

    Circle collisionCircle;

    int velocityX;
    int velocityY;

    SDL_Renderer *renderer;
    LTexture *texture;

    int currentDirection = 0;

    /* METHODS */
    virtual void initializeSpritesData(int x, int y);
};


#endif //TP4_TOWERDEFENSE_ENEMY_H
