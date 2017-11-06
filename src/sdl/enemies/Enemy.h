#ifndef TP4_TOWERDEFENSE_SDL_ENEMY_H
#define TP4_TOWERDEFENSE_SDL_ENEMY_H


#include <vector>
#include <array>
#include "../LTexture.h"
#include "../../common/Point.h"
#include "../Constants.h"
#include "../Circle.h"

class Enemy {
private:
    /* ATRIBUTOS RELACIONADOS CON DIBUJO */
    SDL_Rect walkBox;
    std::array<std::array<SDL_Rect, NUMBER_OF_ENEMY_WALK_DIRECTIONS>, NUMBER_OF_ENEMY_WALK_SPRITES> walkingSprites;
    //SDL_Rect walkingSprites[NUMBER_OF_ENEMY_WALK_DIRECTIONS][NUMBER_OF_ENEMY_WALK_SPRITES];

    SDL_Rect deathBox;
    std::array<std::array<SDL_Rect, NUMBER_OF_ENEMY_DEATH_DIRECTIONS>, NUMBER_OF_ENEMY_DEATH_SPRITES> deathSprites;
            //SDL_Rect deathSprites[NUMBER_OF_ENEMY_DEATH_DIRECTIONS][NUMBER_OF_ENEMY_DEATH_SPRITES];

    bool firstFrameOfDeathRendered = false;
    bool lastFrameOfDeathRendered = false;

    Circle collisionCircle;

    int velocityX;
    int velocityY;

    LTexture texture;
    SDL_Renderer *renderer;

    int currentDirection = 0;

public:
    Enemy(int x, int y, SDL_Renderer *renderer);

    ~Enemy();

    /*
     * Carga la imagen que contiene los sprites del monstruo.
     */
    bool loadMedia();

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
     * que el mapa tiene columnas = CARTES
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
    void setTexture(LTexture &texture);

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


protected:
    // abominable como monstruo default...porque sí... :D
    const std::string TEXTURE_FILE_PATH = "images/sprites/enemy-abominable-walk.png";

    const int FRONT_LEFT_SPRITE_ROW = 1;
    const int BACK_LEFT_SPRITE_ROW = 3;
    const int BACK_RIGHT_SPRITE_ROW = 5;
    const int FRONT_RIGHT_SPRITE_ROW = 7;

    const int SPRITE_DIRECTIONS[4] = {
            FRONT_LEFT_SPRITE_ROW,
            BACK_LEFT_SPRITE_ROW,
            BACK_RIGHT_SPRITE_ROW,
            FRONT_RIGHT_SPRITE_ROW
    };

    const int WALK_SPRITE_WIDTH = 105;
    const int WALK_SPRITE_HEIGHT = 119;

    const int DEATH_SPRITE_WIDTH = 200;
    const int DEATH_SPRITE_HEIGHT = 155;

    const int MAX_VELOCITY = 10;

    bool isAlive;
    int initialLifePoints;
    int remainingLifePoints;
    int velocity;
    int isAir;

    void shiftColliders();

};


#endif //TP4_TOWERDEFENSE_ENEMY_H
