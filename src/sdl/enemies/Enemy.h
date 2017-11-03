#ifndef TP4_TOWERDEFENSE_ENEMY_H
#define TP4_TOWERDEFENSE_ENEMY_H


#include <vector>
#include "../LTexture.h"
#include "../../common/Point.h"
#include "../Constants.h"

const int NUMBER_OF_WALK_SPRITES = 12;
const int NUMBER_OF_WALK_DIRECTIONS = 4;

const int NUMBER_OF_DEATH_SPRITES = 18;
const int NUMBER_OF_DEATH_DIRECTIONS = 4;


class Enemy {
protected:
    // abominable como monstruo default...porque sí (?)
    const std::string TEXTURE_FILE_PATH = "images/sprites/enemy-abominable-walk.png";

    //const int FRONT_SPRITE_ROW = 0;
    const int FRONT_LEFT_SPRITE_ROW = 1;
    //const int LEFT_SPRITE_ROW = 2;
    const int BACK_LEFT_SPRITE_ROW = 3;
    //const int BACK_SPRITE_ROW = 4;
    const int BACK_RIGHT_SPRITE_ROW = 5;
    //const int RIGHT_SPRITE_ROW = 6;
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

private:
    //Collision box of the enemy
    SDL_Rect walkBox;
public:
    const SDL_Rect &getWalkBox() const;

private:
    SDL_Rect deathBox;

    //The velocity of the enemy
    int velocityX, velocityY;

    std::vector<Point> path;

    LTexture &texture;
    SDL_Renderer *renderer;
    SDL_Rect walkingSprites[NUMBER_OF_WALK_DIRECTIONS][NUMBER_OF_WALK_SPRITES];
    SDL_Rect deathSprites[NUMBER_OF_DEATH_DIRECTIONS][NUMBER_OF_DEATH_SPRITES];

    bool isAlive;

public:
    Enemy(int x, int y, SDL_Renderer *renderer, LTexture &t);

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
     * Renderea el enemigo en la pantalla.
     * camera: cuadrado que representa el lugar visible del mapa.
     * clip: el número de clip en el sprite a dibujar.
     * */
    void renderWalk(SDL_Rect &camera);

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

    void renderDie(SDL_Rect &camera);

    void animate(SDL_Rect &camera);

    void kill();
};


#endif //TP4_TOWERDEFENSE_ENEMY_H
