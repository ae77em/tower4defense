#ifndef TP4_TOWERDEFENSE_TOWER_H
#define TP4_TOWERDEFENSE_TOWER_H

#include <vector>
#include "../LTexture.h"
#include "../../common/Point.h"
#include "../Constants.h"
#include "../Circle.h"

const int NUMBER_OF_IDLE_SPRITES = 8;
const int NUMBER_OF_SHOT_SPRITES = 8;

class Tower {
private:
    //Collision box of the enemy
    SDL_Rect idleBox;
    SDL_Rect shotBox;

    LTexture &texture;
    SDL_Renderer *renderer;
    SDL_Rect idleSprites[NUMBER_OF_IDLE_SPRITES];
    SDL_Rect shotSprites[NUMBER_OF_SHOT_SPRITES];

public:
    Tower(int x, int y, SDL_Renderer *renderer, LTexture &t);

    ~Tower();

    /*
     * Carga la imagen que contiene los sprites de la torre.
     */
    bool loadMedia();

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

    /*
     * Modifica la textura que se utilizará para mostrar la torre.
     * texture: referencia a la textura a utilizar.
     * */
    void setTexture(LTexture &texture);

    /*
     * Modifica la cámara del juego donde está metido la torre, es decir,
     * el cuadro que se encarga de mostrar la parte visible del juego.
     * camera: referencia a la cámara a utilizar.
     * */
    void setCamera(SDL_Rect &camera);

    /*
     * Setea el renderer, encargad de dibujar la textura de la torre en el juego.
     * camera: puntero al renderer utilizado.
     * */
    void setRenderer(SDL_Renderer *renderer);


    const SDL_Rect &getIdleBox() const;


    Circle &getCollisionCircle();

    int getShotDamage();

    void setIsShooting(bool isShooting);

    int shotDamage;
protected:
    // abominable como monstruo default...porque sí... :D
    const std::string TEXTURE_FILE_PATH = "images/sprites/enemy-abominable-idle.png";

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

    const int IDLE_SPRITE_WIDTH = 104;
    const int IDLE_SPRITE_HEIGHT = 127;

    const int SHOT_SPRITE_WIDTH = 104;
    const int SHOT_SPRITE_HEIGHT = 127;

    int shotRatio;

    int shotMsTimeGap;

    bool isShooting;

    int lastShotTime = 0;

    void shiftColliders();

    Circle collisionCircle;
};

#endif //TP4_TOWERDEFENSE_TOWER_H
